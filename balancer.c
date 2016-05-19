#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Global variables
// number of vms on each group
#define VMS 3

int total_no_large = VMS;
int total_no_small = VMS;

// intially all the vms are on
int working_large = 0;
int working_small = 0;

// IP list of large VMS

char * list_large[3] = {"0","0","0"};
char * list_small[3] = {"0","0","0"};

// number of requests each vm category can handle
#define unit 5
const float allowed_req_large[2] = {46, 180}; //{10,20}
const float allowed_req_small[2] = { 23,110};// {5,10}

// vm status

int large_vms[3] = {1,1,1};
int small_vms[3] = {1,1,1};

// amount of time each vm is switched off

int time_vm_large_off[3] = {0,0,0};
int time_vm_small_off[3] = {0,0,0};

int time_granularity = 10;


/*float weight_large = (float)allowed_req_large[1]/allowed_req_large[0];
float weight_small = (float)allowed_req_small[1]/allowed_req_small[0];
*/

float weight_large = (float)180/46;
float weight_small = (float)110/23;

char*  vmarray[9] = {    "54.186.167.97",
                 "ec2-54-186-167-97.us-west-2.compute.amazonaws.com",

                 "54.201.215.21",
                 "ec2-54-201-215-21.us-west-2.compute.amazonaws.com",

                 "54.200.27.251",
                 "ec2-54-200-27-251.us-west-2.compute.amazonaws.com",

                 "54.191.118.176",
                 "54.149.239.53",
                 "54.191.78.166"
                };
 
int switch_on (int large, int vm_no){
	if(large) {
		if(!large_vms[vm_no]){ //the vm is off
			//update time counter and switch the flag
			time_vm_large_off[vm_no] += time_granularity;
			large_vms[vm_no] = 1;
			//use curl to switch on the load balancer
			char command[1000];
			char * input = "%3A%2F%2F";
			sprintf(command,"curl -d \"w_lf=1&w_ls=0&w_wr=&w_rr=&w_status_I=0&w_status_N=0&w_status_D=0&w_status_H=0&w=http%s%s&b=eec_balancer&nonce=da52d469-9748-4dc1-8f4e-b7eff277f188\" http://54.186.116.220/balancer-manager",input,vmarray[vm_no*2]);
			system(command);
			 sprintf(command,"curl -d \"w_lf=1&w_ls=0&w_wr=&w_rr=&w_status_I=0&w_status_N=0&w_status_D=0&w_status_H=0&w=http%s%s&b=eec_balancer&nonce=da52d469-9748-4dc1-8f4e-b7eff277f188\" http://54.186.116.220/balancer-manager",input,vmarray[vm_no*2+1]); 
			system(command);
			
		}		
	} else {
		if(!small_vms[vm_no]) {
			//update time counter and switch the flag               
			time_vm_small_off[vm_no] += time_granularity;
                        small_vms[vm_no] = 1;         
			char command[1000];                                                                                              
			char * input = "%3A%2F%2F";                                                       
                        sprintf(command,"curl -d \"w_lf=1&w_ls=0&w_wr=&w_rr=&w_status_I=0&w_status_N=0&w_status_D=0&w_status_H=0&w=http%s%s&b=eec_balancer&nonce=da52d469-9748-4dc1-8f4e-b7eff277f188\" http://54.186.116.220/balancer-manager",input,vmarray[vm_no+6]);                                                                                                                       			printf("Running command %s",command);
                        system(command);                                             
		}
	}
}

int print_array(){
	int i=0;
	printf("\nLarge VMS : ");
	for(i=0;i<3;i++) 
		printf("%d ",large_vms[i]);
	printf("\nSmall VMS : ");
	for(i=0;i<3;i++) 
		printf("%d ",small_vms[i]);
	printf("\n");
}

int switch_off (int large, int vm_no){
	if (large) {
		if(large_vms[vm_no]){
			//the vm is on, switch it off
			large_vms[vm_no] = 0;
			//use curl to switch off
                        char command[1000];                                                                                                                                                    
			char * input = "%3A%2F%2F";; 
                        sprintf(command,"curl -d \"w_lf=1&w_ls=0&w_wr=&w_rr=&w_status_I=0&w_status_N=0&w_status_D=1&w_status_H=0&w=http%s%s&b=eec_balancer&nonce=da52d469-9748-4dc1-8f4e-b7eff277f188\" http://54.186.116.220/balancer-manager",input,vmarray[vm_no*2]);                                                                                                                            
                        system(command);                                                                                                                                                        
                        sprintf(command,"curl -d \"w_lf=1&w_ls=0&w_wr=&w_rr=&w_status_I=0&w_status_N=0&w_status_D=1&w_status_H=0&w=http%s%s&b=eec_balancer&nonce=da52d469-9748-4dc1-8f4e-b7eff277f188\" http://54.186.116.220/balancer-manager",input,vmarray[vm_no*2+1]);
                        system(command);
			printf("\noff_large %s %s %s",vmarray[vm_no*2],vmarray[vm_no*2+1],command);
			
		}else {
			//the vm is already off, just update the time counter
			time_vm_large_off[vm_no] += time_granularity;
		}
	} else {
		if (small_vms[vm_no]){
			//the vm is on, turn it off
			small_vms[vm_no] = 0;
                        char command[1000];
			char * input = "%3A%2F%2F";
                        sprintf(command,"curl -d \"w_lf=1&w_ls=0&w_wr=&w_rr=&w_status_I=0&w_status_N=0&w_status_D=1&w_status_H=0&w=http%s%s&b=eec_balancer&nonce=da52d469-9748-4dc1-8f4e-b7eff277f188\" http://54.186.116.220/balancer-manager",input,vmarray[vm_no+6]);
                        system(command);
			printf("\noff_small %s",command);

		} else {
			// the vm is off, update the counter
			time_vm_small_off[vm_no] +=time_granularity;
		}
	}
	
}


//Seperate Load balancer for each vm size

int load_balancer1(int count1, int count2){
	//load balance for large vms
	int i=0;
	for (i=0; i < total_no_large; i++) {
		if(count1 == 0 ){
			switch_off(1,i);
		}else if(count1 > time_granularity * allowed_req_large[0]){
			switch_on(1,i);
			count1 -= time_granularity * allowed_req_large[0];
		}else {
			switch_on(1,i);
			count1 = 0;
		}
	}
	
	//load balance for small vms
	for (i=0; i< total_no_small; i++) {
		if(count2 == 0) {
			switch_off(0,i);
		} else if (count2 > time_granularity  * allowed_req_small[1]) {
			switch_on(0,i);
			count2 -= time_granularity * allowed_req_small[1];
		} else {
			switch_on(0,i);
			count2 = 0;
		}
	}
	if(count1 > 0 || count2 > 0) {
		return -1;
	} else {
		return 0;
	}
}


int load_balancer3(int count1, int count2){
	int i;
	for(i=0;i<3;i++) {
	        if(count1 >0 && count1 >= time_granularity * allowed_req_large[0]){
	                count1 -=  time_granularity * allowed_req_large[0];
			switch_on(1,i);
                }else if(count2 > 0) {
                	if((time_granularity * allowed_req_large[1]) - (count1* weight_large) >= count2) {
                        	count2 =0;
                        	count1 =0;
				switch_on(1,i);
                	}else {
                        	count2 = count2 - ((time_granularity * allowed_req_large[1]) - (count1* weight_large));
                       	 	count1 =0;
				switch_on(1,i);
                	}
        	}else if(count1 == 0 && count2 ==0)
			switch_off(1,i);
		else {
			switch_on(1,i);
			count1=0;
		}
	}
	int normalized_count = count1 * weight_small + count2;
        for (i=0; i< total_no_small; i++) {
                if(normalized_count == 0) {
                        switch_off(0,i);
                } else if (normalized_count > time_granularity  * allowed_req_small[1]) {
                        switch_on(0,i);
                        normalized_count -= time_granularity * allowed_req_small[1];
                } else {
                        switch_on(0,i);
                        normalized_count = 0;
                }
        }
	if(normalized_count > 0)
		return -1;
	return 0;
}
//100, 75
float large_power = 2;
float small_power = 1;
/*
 * const float allowed_req_large[2] = {46, 180}; //{10,20}
 * const float allowed_req_small[2] = { 23,110};// {5,10}
 *
*/
//normalize cpu intensive application to the other one
float load_balancer2(int count1, int count2,int * result){
	if (count1 ==0 && count2 == 0) return 0;
	// assign large
        int left_result[2] = { 0, 0};
        int right_result[2] = {0,0};
        float power_l = large_power;
        float power_s = small_power;

	if(working_large < 3 ){
	int lcount1 = count1;
	int lcount2 = count2;
	//int normalized_count = lcount1 * weight_large + lcount2;
	if(lcount1 >= time_granularity * allowed_req_large[0]) 
		lcount1 -=  time_granularity * allowed_req_large[0];
	else {
		if((time_granularity * allowed_req_large[1]) - (lcount1* weight_large) >= lcount2) {
			lcount2 =0;
			lcount1 =0; 
		}else {
			lcount2 = lcount2 - ((time_granularity * allowed_req_large[1]) - (lcount1* weight_large));
			lcount1 =0;
		}
	} 
	
	working_large++;
	power_l+= load_balancer2(lcount1,lcount2, left_result);
	working_large--;
	} else {
		power_l = 10000;
	}
	//assign small
	if(working_small <3) {
	int scount1 = count1;
        int scount2 = count2;
        //normalized_count = scount1 * weight_small + scount2;
        if(scount1 >= time_granularity  * allowed_req_small[0])
                scount1 -=  time_granularity * allowed_req_small[0];
        else {
                if((time_granularity * allowed_req_small[1]) - (scount1* weight_small) >= scount2) {                                                                                       
                        scount2 =0;                                                                                                                                                             
                        scount1 =0;                                                                                                                                                             
                }else {
                        scount2 = scount2 - ((time_granularity  * allowed_req_small[1]) - (scount1* weight_small));                                                                           
                        scount1 =0;                                                                                                                                                             
                }
        }

	working_small++;
	power_s += load_balancer2(scount1,scount2,right_result);
	working_small--;
	} else {
		power_s = 100000;
	}
	if(power_l > power_s){
		 result[0] = right_result[0];
		 result[1] = right_result[1] + 1; 
		 return power_s;
	} else {
		result[0] = left_result[0] + 1;
		result[1] = left_result[1];
		return power_l;
	}

}

int load_balancer3_helper(int count1, int count2) {
	
        FILE *filePtr;
        int power = load_balancer3(count1, count2);

        int large = large_vms[0] + large_vms[1] + large_vms[2];
        int small = small_vms[0] + small_vms[1] + small_vms[2];
        filePtr = fopen("/etc/httpd/conf/status_3.txt","a");                                                                                                                                     
        if(filePtr==NULL){
                printf("Error creating status file. . !");
        }
        else{
                if(power < 0)
                {
                                fprintf(filePtr,"SLA Violation %d \n",large*2 + small);
                }
                else{
                                fprintf(filePtr,"%d \n",large*2 + small);
                }
        }
        fclose(filePtr);
}

int load_balancer2_helper(int count1, int count2) {
        int power;
        int res[2] = {0,0};
        FILE *filePtr;

        power = load_balancer2(count1, count2, res);

        filePtr = fopen("/etc/httpd/conf/status_2.txt","a");                                                                                                                                                                                                                    
        if(filePtr==NULL){
                printf("Error creating status file. . !");
        }
        else{
                fprintf(filePtr,"Number of large_cpu %d \t small cpu %d \t power %d\n", res[0], res[1], power);
		fprintf(filePtr,"%d \n",power);
		int i=0;
		for(i=0;i<3;i++){
			if(i<res[0])
				switch_on(1,i);
			else
				switch_off(1,i);
		}
		for(i=0;i<3;i++){
			if(i<res[1])
				switch_on(0,i);
			else
				switch_off(0,i);
		}
        }
        fclose(filePtr);
}

int load_balancer1_helper(int count1, int count2) {
        int power;
        FILE *filePtr;


        power = load_balancer1(count1, count2);
		
        int large = large_vms[0] + large_vms[1] + large_vms[2];
        int small = small_vms[0] + small_vms[1] + small_vms[2];
        filePtr = fopen("/etc/httpd/conf/status_1.txt","a");                                                                                                                                                                                                                    
        if(filePtr==NULL){
                printf("Error creating status file. . !");
        }
        else{
		if(power < 0)
		{
				//fprintf(filePtr,"SLA Violation : Number of large_cpu %d \t small cpu %d \t power %d\n", large, small, large*2 + small);
				fprintf(filePtr,"SLA Violation %d \n",large*2 + small);
		}
		else{
                		//fprintf(filePtr,"Number of large_cpu %d \t small cpu %d \t power %d\n", large, small, large*2 + small);
				fprintf(filePtr,"%d \n",large*2 + small);
		}
        }
        fclose(filePtr);
}

int main(int argc,char *argv[])
{
	FILE *fp, *fp2;
	int pos = 0;
	int res[2] = {0,0};
	
	while(1) {
		sleep(10);
		fp = fopen("/etc/httpd/logs/access_log","r");
		int line_count_recent= 0;
		int count_app1 =0, count_app2 = 0;
		char buf[1024];
		fseek(fp,pos,0);

		while (fgets(buf,1024,fp)) {
			if(strstr(buf,"prime200"))
				count_app1++;
			else if(strstr(buf,"prime100"))
				count_app2++;
		}

		printf("Count: %d , %d\n",count_app1,count_app2);
		pos = ftell(fp); 
		load_balancer2_helper(count_app1, count_app2);
		count_app1 = 0;
		count_app2 = 0;
		fclose(fp);
	}
	
	//load_balancer1_helper(atoi(argv[1]),atoi(argv[2]));
	//load_balancer2_helper(atoi(argv[1]),atoi(argv[2]));

	return 0;
}
