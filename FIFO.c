
    #include <stdio.h>
    #include <stdlib.h>

    /* Process Data Structure */
    struct page {
    int pid; /* Process ID */
    int accesstime; /* When access was requested */

    int ref; /*referenced */
    int modif; /* Waiting time, for round-robin scheduling */
    struct propagecess *next;


    };

    struct page *init_page (int pid, int acesstime, int ref,int modif) {
       struct page *p;
       p = malloc(sizeof(struct page));
       if (p == NULL) {
       printf("You gave a wrong process info.\n");
       exit(1);
       };
       p->pid = pid;
       p->accesstime = acesstime;
       p->ref = ref;
       p->modif = modif;
       p->next = NULL;

       return(p);
       };


    void listpages (struct page *p) {
    struct page *tmp = p;

    printf("BEGIN:\tPage Listing\n");

    while (tmp != NULL) {
    printf("PID: %d\t\tAccesstime: %d\n", tmp->pid,  tmp->accesstime);
    tmp = tmp->next;
    };

    printf("END:\tPage Listing\n\n");
    };







#define MAXCHAR 1000
   int main (void) {
	   struct page *plist, *ptmp;



	   int time, id;
	  		       char *x;
	  		       int  i=0, r;

	  		       FILE *fp;
	  		       fp = fopen("/home/romy/pages.txt", "rt");
	  		       if (fp == NULL)
	  		           {
	  		           printf("File does not exist");
	  		           exit(1);
	  		           }

	  		       while ((r = fscanf(fp, "%d,%d,%s", &time, &id,x)) != EOF) {
	  		           if (r == 3) {
	  		           i++;

	  		           /*printf("Current value: \t\t%d\n", i);
	  		           printf("Minimum value: \t\t%d\n", id);
	  		           printf("Maximum value: \t\t%d\n", time);
	  		           printf("String value: \t\t%s\n", x);*/
	  		         if(i==1){
	  		        	 if(strcmp(x,"R")==0){

	  		       	  	 plist = init_page(id,time, 1,0);
	  		       	  	 ptmp=plist;



	  		       	  	}
	  		       	  else{
	  		       	  	plist = init_page(id,time,0, 1);

	  		       	  	ptmp=plist;



	  		       	   }

	  		       	 }

	  		       else if(i==2){
	  		   	  		 if(strcmp(x,"R")==0){

	  		   	  		   plist->next = init_page(id,time, 1,0); ptmp=plist->next;



	  		   	  		 }
	  		   	  		 else{

	  		   	  		plist->next = init_page(id,time,0, 1);ptmp=plist->next;


	  		   	  		 }


	  		   	  	}

	  		       else {
	  		   	  		 if(strcmp(x,"R")==0){

	  		   	  		   ptmp->next = init_page(id,time, 1,0); ptmp=ptmp->next;



	  		   	  		 }
	  		   	  		 else{

	  		   	  		ptmp->next = init_page(id,time,0, 1);ptmp=ptmp->next;





	  		   	  		 }


	  		   	  	}

	  		           }

	  		       }


	  		       fclose(fp);



	   FIFO(plist);
	       return 0;



};



  void FIFO(struct page *p){


	  struct page *copy, *tmpsrc,*current,*memory,*signalcopy;

 	  FILE *f = fopen("FIFO output.txt", "w");

	  tmpsrc = p;
	  	   copy = NULL;
	  int pagefault=0;
	  int time=0;
	  int inside=0;
      char* chars[3000];
      int i=0;
		    //printf("BEGIN:\tFIFO (Quantum: %d)\n", 20);


	   while (tmpsrc != NULL) {


	   copy = init_page(tmpsrc->pid, tmpsrc->accesstime, tmpsrc->ref,tmpsrc->modif); //Current Page to load

	   if(time==copy->accesstime){

		   current=memory;
		   int found=0;
          while(current!=NULL){
        	  if(current->pid==copy->pid){
        		  found=1;
        		  if(current->ref==0){

        			  current->ref=copy->ref;
        		  }
        		  if(current->modif==0){
        			  current->modif=copy->modif;
        		  }
        		  break;
        	  }
        	  current=current->next;
          }


          if(found==0){
    		  //printf("Inserting");

        	  pagefault+=1;
        	  if(memory==NULL){
        		  inside=1;
        		  memory=copy;


        	  }
        	  else if(inside<5){
        		  current=memory;
        		  inside++;
        		  while(current->next!=NULL){
        			  current=current->next;
        		  }
        		  current->next=copy;
        	  }
        	  else{
        		  if(memory->modif==0){
        			  printf("Modified page with  (PageID: %d) was kicked \n", memory->pid);


        			  fprintf(f,"Modified page with  (PageID: %d) was kicked \n", memory->pid);

        		  }
        		  memory=memory->next;
        		  current=memory;
        		  inside++;
        		   while(current->next!=NULL){
        		      current=current->next;
        		  }
        		   current->next=copy;



        	  }

          }
          else{
        	  found=0;
          }



          tmpsrc = tmpsrc->next;



	   }
	   printf("At time: %d\tAnd the page fault so far is %d\n", time,pagefault);
	   fprintf(f,"Time: %d\tPageFaults %d\n", time,pagefault);


	   time++;


	   current=memory;
	   while(current!=NULL){
		   printf("Page: %d\tReferenced : %d\tModified: %d\n", current->pid,current->ref , current->modif );
		   fprintf(f,"Page: %d\tReferenced: %d\tModified: %d\n", current->pid,current->ref , current->modif);

		   current=current->next;
	   }
        if(time%19==0){
        	current=memory;
        	 printf("Clock Interrupt at time %d Happened Reset all referencing \n",time);
        	 fprintf(f,"Clock Interrupt Happened at time %d Reset all referencing \n",time);

        	while(current!=NULL){
        		current->ref=0;
        printf("Page: %d\tReferenced : %d\tModified: %d\n", current->pid,current->ref , current->modif );
		   fprintf(f,"Page: %d\tReferenced: %d\tModified: %d\n", current->pid,current->ref , current->modif);
        		current=current->next;


        	}




        }






	   };




    	  fclose(f);









  }
