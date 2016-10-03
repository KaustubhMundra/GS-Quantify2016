//-------------------------------------------------------------------
// We have used in this code Class objects to represent the job and the
// director. The class job has all the information of a job within
// it. Member functions input and display, take input and present
// output for a job object respectively. The most important is the class
// director where we have used List containers to store the job objects.
// We  have preferred List over vectors. This is due to that we have used 
// very frequently insertion and deletion which occur in constant time
// in List against O(n) (which is time efficient)time in vectors.


// The assign part works dynamically as: A list for the jobs is created
// dynamically and stored in the decreasing priority order(in O(n) time) 
// using the compare member Function in the director class. Each time a job
// is added to the job list the number of jobWaiting counter is increased
// by 1.Whenever an assign of job occurs jobWaiting is decreased by 1 per
// job, and number of freeCPU is decreased by 1. These variables also run
// dynamically.The jobs assigned from the previous assignment are stored
// in another List assignJobs and removed from the job list, during which
// the data member completionTime for the job objects are set. Now, 
// whenever an ASSIGN IS ENCOUNTERED,THE PREVIOUSLY HELD ASSIGNJOBS LIST
// IS TRAVERESED AND COMPLETED JOB ARE REMOVED FROM THE ASSIGNJOBS LIST,
// (which is memory efficient)
// INCREASING THE COUNTER FOR THE FREECPU BY 1 EACH TIME. This gives us
// the number of freeCPU at that time and using this we shift the jobs 
// from the job list to assignJobs list, initialising the job object's 
// completionTime data member and displaying them.This process goes on.

// Working Of query:
// The queue is somewhat similar. It has its own copies of new list 
// as it works on history and needs the input order.The input order 
// is stored in a list of structures, which is maintained DYNAMICALLY,
// i.e. non required input, the past inputs are deleted.Only the input 
// between the last query timestamp till the latest timestamp assign 
// and job commands are stored.
// The same way as assign works the list queryList is made as the list
// jobs and the list queryAssignList is made as the list assignJobs
// as discussed previously. There is a list of input strucutes that 
// can be job submission or assignment depending upon the value 
// attribute.Whenever a query is called the previously held queryList
// is operated upon by the dynamically maintained inputList and 
// we get the list formed at the required timestamp.We delete the previous
// inputs to save memory.This goes on dynamically.
// query2() is exactly similar to query but for finding originating sys.
// As it works in different timestamps not relatable to query, we had
// to make it whole new but same to query. 


// Worst case Time-complexities are-
// **Time-complexity for various member functions in Class Job are-
// *input()-constant time.
// *display()-constant time.

// **Time-complexity for various member functions in Class Job are-
// *addNewJob() : O(n) per job input, n=number of jobs.
// *compare() : constant time.
// *getmin() : constant time.
// *assign() : O(n) , n=number of cpus.
// *query() : O(n1*n2), n1=number of cpus, n2= totat number of job inputs.
// *query2() : same as query().
// *queryDisplay2() : O(n): n=number of jobs.
// *queryGetMin() : constant time.
// *queryDisplay() : same as queryDisplay2().
//-------------------------------------------------------------------




#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <list>
#include <cstring>
#include <string>
#include <cctype>
using namespace std;

unsigned long long int cpu;        //number of cpus

struct inputList{                     //structure to store input, the structure list is dynamically created.
	int value;
	unsigned long long int timestamp;
	unsigned long long int id;
	string orig;
	string instr;
	unsigned int importance;
	unsigned long int duration;

	unsigned long long int assignTimestamp;
	unsigned long long int assignK;
};

class job{							//each job is submitted creating a job object of this class
	unsigned long long int timestamp;
	unsigned long long int id;
	string orig;
	string instr;
	unsigned int importance;
	unsigned long int duration;
	unsigned long long int completionTime;
public:
	job()
	{

	}
	job(unsigned long long int t,unsigned long long int i,string o,string in,unsigned int imp,unsigned long int d) //parametric constructor for initialisation directly
	{
		timestamp=t;
		id=i;
		orig=o;
		instr=in;
		importance=imp;
		duration=d;
	}
	void input()					//input of the job object , i.e job submission to the director
	{
		scanf("%llu",&(timestamp));
		scanf("%llu",&(id));
		cin>>orig;
		cin>>instr;
		scanf("%u",&(importance));
		scanf("%lu",&(duration));
	}
	unsigned long long int giveId()   //methods to acces private variables outside class
	{
		return id;
	}
	string giveOrig()
	{
		return orig;
	}
	string giveInstr()
	{
		return instr;
	}
	unsigned int giveImportance()
	{
		return importance;
	}
	unsigned long long int giveTimestamp()
	{
		return timestamp;
	}
	unsigned long int giveDuration()
	{
		return duration;
	}
	unsigned long long int giveCompletionTime()
	{
		return completionTime;
	}
	void setCompletionTime(unsigned long long int assignTime)  //sets the completion time for a job once its been assigned using the time of assgin as parameter
	{
		completionTime=assignTime+duration;
	}
	void display() 											//display of a single job object info
	{
		cout<<"job "<<timestamp<<" "<<id<<" "<<orig<<" "<<instr<<" "<<importance<<" "<<duration;
	}
};
class compareQ{

public:
	bool operator ()(job &job2,job &job1)
	{
		if(job1.giveImportance()>job2.giveImportance())
			return true;
		if(job1.giveImportance()<job2.giveImportance())
			return false;
		if(job1.giveTimestamp()<job2.giveTimestamp())
			return true;
		if(job1.giveTimestamp()>job2.giveTimestamp())
			return false;
		if(job1.giveDuration()< job2.giveDuration())
			return true;
		if(job1.giveDuration()> job2.giveDuration())
			return false;
		return false;
	}
};
class director{				//this class maintains all that is done, it behaves as actual director
//list <job> jobs;			//used list over vector as poping and pushing possible very fast than vector,list of jobs submitted in priority order maitained dynamically
list <job> assignJobs;		//list of jobs currently running in cpu maintained dynamically
priority_queue <job,vector<job>,compareQ> jobs;
//priority_queue <job,vector<job>,compare> assignJobs;
//list <job> queryList;			//same as jobs but for query
priority_queue <job,vector<job>,compareQ> queryList;
list <job> queryAssignList;			//same as assignJobs but for query
vector <inputList> inputTaken; 		//input list maintained dynamically
//list <job> queryList2;				//same as queryList but for orignating sys query
priority_queue <job,vector<job>,compareQ> queryList2;
list <job> queryAssignList2;			//same as queryAssignList but for originating sys
//list <inputList> inputTaken2;			//similar input list as inputTaken, but for query over originating sys
unsigned long long int jobWaiting;			//jobs in queue maintained dynamically
unsigned long long int freeCPU;
unsigned long long int queryJobWaiting;			//number of free cpu maintained dynamically
unsigned long long int queryFreeCPU;
unsigned long long int queryFreeCPU2;
unsigned long long int queryJobWaiting2;
public:
	director(unsigned long long int cpu)			//constuctor to initialize values
	{
		jobWaiting=0;
		freeCPU=cpu;
		queryJobWaiting=0;
		queryFreeCPU=cpu;
		queryJobWaiting2=0;
		queryFreeCPU2=cpu;
	}

	void addNewJob(job newJ)					//function to add the object newJ to list jobs
	{
		// list <job> :: iterator it;
		// int check=0;
		// for(it=jobs.begin();it!=jobs.end();++it)
		// {
		// 	if(compare(newJ,(*it))==1)
		// 	{
		// 		jobs.insert(it,newJ);
		// 		check=1;
		// 		break;
		// 	}	
		// }
		// if(!check)
		// 	jobs.push_back(newJ);
		jobs.push(newJ);
		jobWaiting++;

		inputList info;
		info.value=1;
		info.timestamp=newJ.giveTimestamp();
		info.id=newJ.giveId();
		info.orig=newJ.giveOrig();
		info.instr=newJ.giveInstr();
		info.importance=newJ.giveImportance();
		info.duration=newJ.giveDuration();
		inputTaken.push_back(info);
		//inputTaken2.push_back(info);

	}

	// int compare(job job1,job job2)     //comparing two objects based on given preference rules
	// {
	// 	if(job1.giveImportance()>job2.giveImportance())
	// 		return 1;
	// 	if(job1.giveImportance()<job2.giveImportance())
	// 		return 2;
	// 	if(job1.giveTimestamp()<job2.giveTimestamp())
	// 		return 1;
	// 	if(job1.giveTimestamp()>job2.giveTimestamp())
	// 		return 2;
	// 	if(job1.giveDuration()< job2.giveDuration())
	// 		return 1;
	// 	if(job1.giveDuration()> job2.giveDuration())
	// 		return 2;
	// 	return -1;	
	// }
	unsigned long long int  getmin(unsigned long long int a,unsigned long long int b,unsigned long long int c)   //min of 3 variables
	{
		if(a<b && a<c)
			return a;
		else if(b<a && b<c)
			return b;
		else
   		return c;
	}
	void assign(unsigned long long int tStamp,unsigned long long int k)  			//function for assign operation
	{
		list <job>::iterator it (assignJobs.begin());
		list <job>::iterator const end (assignJobs.end());
		for(;it!=end;++it)				//finds the jobs completed from prev assignJobs list and removes the completed jobs
		{
			if((*it).giveCompletionTime()<=tStamp)
			{
				freeCPU++;
				it=assignJobs.erase(it);
				it--;
			}
		}
		// unsigned long long int minAssign;
		// minAssign=getmin(freeCPU,jobWaiting,k);
		//important step as decides the structure of assignJobs array whether keeping the new jobs in front or back of array, right now back
		// list <job>:: iterator it2;
		// it=it2=jobs.begin();
		// advance(it,minAssign);
		// for(;it2!=it;++it2)								//adds the required number of jobs to assignJobs list
		// {
		// 	(*it2).setCompletionTime(tStamp);
		// 	(*it2).display();
		// 	cout<<endl;
		// 	assignJobs.insert(assignJobs.end(),*it2);
		// }
		// it2=jobs.begin();
		// jobs.erase(it2,it);								//removes the assigned jobs from jobs(dynamic queue) list.
		// freeCPU=freeCPU-minAssign;
		// jobWaiting=jobWaiting-minAssign;
		unsigned long long int removed=0;
		unsigned long long int s=jobWaiting;
		//list <job>:: iterator it2;
		while(1)
		{
			if(removed<k && removed<s && freeCPU>0)
			{
				// (*it2).setCompletionTime(tStamp);
				// (*it2).display();
				job j;
				j=jobs.top();
				jobs.pop();
				j.setCompletionTime(tStamp);
				j.display();
				cout<<endl;
				jobWaiting--;
				if((j).giveDuration()!=0)
					freeCPU--;
				removed++;
				assignJobs.push_back(j);
			}
			else
				break;

		}
		//it2=jobs.begin();
		// list <job>:: iterator it3;
		// it3=jobs.begin();
		// advance(it3,removed);
		// jobs.erase(jobs.begin(),it3);

		//updating input list
		inputList info;									//stores the input order
		info.value=-1;
		info.assignTimestamp=tStamp;
		info.assignK=k;
		inputTaken.push_back(info);
		//inputTaken2.push_back(info);
	}
	void query(unsigned long long int tStamp,unsigned long long int k)
	{
		vector <inputList> :: iterator it (inputTaken.begin());
		for(;it!=inputTaken.end();++it)		//iterates over the input list till the latest query call and removes the inputs traversed to save memory
		{
			if((*it).value ==1 )				//value 1 indicates job has been submitted
			{
				if((*it).timestamp <= tStamp)		//basic addNewJob function type
				{
					job newJ((*it).timestamp,(*it).id,(*it).orig,(*it).instr,(*it).importance,(*it).duration);
					// list <job> :: iterator it2;
					// int check=0;
					// for(it2=queryList.begin();it2!=queryList.end();++it2)
					// {
					// 	if(compare(newJ,(*it2))==1)
					// 	{
					// 		queryList.insert(it2,newJ);
					// 		check=1;
					// 		break;
					// 	}	
					// }
					// if(!check)
					// 	queryList.push_back(newJ);
					queryList.push(newJ);
					queryJobWaiting++;
					// it=inputTaken.erase(it);				//processed input removed
					// it--;

				}
				else
					break;

			}
			else			//indicates stored input as assign
			{

				if((*it).assignTimestamp <=tStamp)
				{
					list <job>::iterator it1 (queryAssignList.begin());
					list <job>::iterator const end (queryAssignList.end());
					for(;it1!=end;++it1)    //basic assign step as done earlier in assign
					{
						if((*it1).giveCompletionTime()<=(*it).assignTimestamp)
						{
							queryFreeCPU++;
							it1=queryAssignList.erase(it1);
							it1--;
						}
					}
					// unsigned long long int minAssign;
					// minAssign=getmin(queryFreeCPU,queryJobWaiting,(*it).assignK);
					// list <job>:: iterator it3;
					// list <job>:: iterator it4;
					// it3=it4=queryList.begin();
					// advance(it4,minAssign);
					// for(;it3!=it4;++it3)
					// {
					// 	(*it3).setCompletionTime((*it).assignTimestamp);
					// 	queryAssignList.insert(queryAssignList.end(),*it3);
					// }
					unsigned long long int removed=0;
					unsigned long long int s=queryJobWaiting;
					//list <job>:: iterator it2;
					while(1)
					{
						if(removed<(*it).assignK && removed<s && queryFreeCPU>0)
						{
							job j;
							j=queryList.top();
							queryList.pop();
							j.setCompletionTime((*it).assignTimestamp);
							//(*it2).setCompletionTime((*it).assignTimestamp);
							//(*it2).display();
							//cout<<endl;
							queryJobWaiting--;
							if(j.giveDuration()!=0)
								queryFreeCPU--;
							removed++;
							//queryAssignList.insert(queryAssignList.end(),j);
							queryAssignList.push_back(j);
						}
						else
							break;

					}
					//it2=jobs.begin();
					// list <job>:: iterator it3;
					// it3=queryList.begin();
					// advance(it3,removed);
					// queryList.erase(queryList.begin(),it3);


					// it3=queryList.begin();
					// queryList.erase(it3,it4);				
					// queryFreeCPU=queryFreeCPU-minAssign;
					// queryJobWaiting=queryJobWaiting-minAssign;


					// it=inputTaken.erase(it);		//processed input removed
					// it--;

				}
				else
					break;
			}
		}
		queryDisplay(k,queryJobWaiting);
		queryList = priority_queue<job,vector<job>,compareQ>();
		queryAssignList.clear();
		queryJobWaiting=0;
		queryFreeCPU=cpu;
	}


	void query2(unsigned long long int tStamp,string queryOrig2)  // exactly the same as query
	{
		vector <inputList> :: iterator it (inputTaken.begin());
		for(;it!=inputTaken.end();++it)
		{
			if((*it).value ==1 )
			{
				if((*it).timestamp <= tStamp)
				{
					job newJ((*it).timestamp,(*it).id,(*it).orig,(*it).instr,(*it).importance,(*it).duration);
					// list <job> :: iterator it2;
					// int check=0;
					// for(it2=queryList2.begin();it2!=queryList2.end();++it2)
					// {
					// 	if(compare(newJ,(*it2))==1)
					// 	{
					// 		queryList2.insert(it2,newJ);
					// 		check=1;
					// 		break;
					// 	}	
					// }
					// if(!check)
					// 	queryList2.push_back(newJ);
					queryList2.push(newJ);
					queryJobWaiting2++;
					// it=inputTaken2.erase(it);
					// it--;

				}
				else
					break;

			}
			else
			{

				if((*it).assignTimestamp <=tStamp)
				{
					list <job>::iterator it1 (queryAssignList2.begin());
					list <job>::iterator const end (queryAssignList2.end());
					for(;it1!=end;++it1)
					{
						if((*it1).giveCompletionTime()<=(*it).assignTimestamp)
						{
							queryFreeCPU2++;
							it1=queryAssignList2.erase(it1);
							it1--;
						}
					}
					// unsigned long long int minAssign;
					// minAssign=getmin(queryFreeCPU2,queryJobWaiting2,(*it).assignK);
					// list <job>:: iterator it3;
					// list <job>:: iterator it4;
					// it3=it4=queryList2.begin();
					// advance(it4,minAssign);
					// for(;it3!=it4;++it3)
					// {
					// 	(*it3).setCompletionTime((*it).assignTimestamp);
					// 	queryAssignList2.insert(queryAssignList2.end(),*it3);
					// }

					unsigned long long int removed=0;
					unsigned long long int s=queryJobWaiting2;
					//list <job>:: iterator it2;
					while(1)
					{
						if(removed<(*it).assignK && removed<s && queryFreeCPU2>0)
						{
							job j;
							j=queryList2.top();
							queryList2.pop();
							(j).setCompletionTime((*it).assignTimestamp);
							// (*it2).display();
							// cout<<endl;
							queryJobWaiting2--;
							if(j.giveDuration()!=0)
								queryFreeCPU2--;
							removed++;
							queryAssignList2.push_back(j);
						}
						else
							break;

					}
					//it2=jobs.begin();
					// list <job>:: iterator it3;
					// it3=queryList2.begin();
					// advance(it3,removed);
					// queryList2.erase(queryList2.begin(),it3);


					// it3=queryList2.begin();
					// queryList2.erase(it3,it4);
					// queryFreeCPU2=queryFreeCPU2-minAssign;
					// queryJobWaiting2=queryJobWaiting2-minAssign;


					// it=inputTaken2.erase(it);
					// it--;

				}
				else
					break;
			}
		}
		queryDisplay2(queryOrig2);
		queryList2= priority_queue <job, vector<job>,compareQ>();
		queryAssignList2.clear();
		queryJobWaiting2=0;
		queryFreeCPU2=cpu;
	}
	void queryDisplay2(string orig)
	{
		while(!queryList2.empty())
		{
			job j;
			j=queryList2.top();
			if((j.giveOrig()).compare(orig)==0)
			{
				(j).display();
				cout<<endl;
			}
			queryList2.pop();
		}
	}
	unsigned long long int queryGetMin(unsigned long long int a,unsigned long long int b)
	{
		if(a>b)
			return b;
		else
			return a;
	}
	void queryDisplay(unsigned long long int k,unsigned long long int s)
	{
		unsigned long long min,counter=0;
		min=queryGetMin(k,s);

		while(!queryList.empty())
		{
			job j;
			j=queryList.top();
			queryList.pop();
			j.display();
			cout<<endl;
			counter++;
			if(counter==min)
			{
				break;
			}

		}

	}
	// void display()
	// {
	// 	list <job> :: iterator it;
	// 	for(it=jobs.begin();it!=jobs.end();++it)
	// 	{
	// 		(*it).display();
	// 		cout<<endl;
	// 	}
	// }
};
int main() {
	unsigned long long int timestamp;
	unsigned long long int k;
	char dummy[10];
	scanf("%s",dummy);
    scanf("%llu",&cpu);
    director dir(cpu);
    //cout<<cpu;
    char input[10];
    int t;
    char c;
    while(1)
    {
    	t=scanf("%s",input);
    	if(t==-1)					//scanf return -1 for eof
    		break;
    	else{
    		if(strcmp(input,"job")==0)
    		{
    			job newJ;
    			newJ.input();
				dir.addNewJob(newJ);
	    	}
    		else if(strcmp(input,"assign")==0)
    		{
    			scanf("%llu",&timestamp);
    			scanf("%llu",&k);
    			dir.assign(timestamp,k);
    		}
    		else if(strcmp(input,"query")==0)
    		{
    			int queryCase=1;
    			unsigned long long int timestamp;
    			scanf("%llu",&timestamp);
    			string input;
    			cin>>input;
    			string :: iterator it;
    			for(it=input.begin();it!=input.end();it++)
    			{
    				if(isalpha(*it))
    				{
    					queryCase=2;
    					break;
    				}
    			}
    			if(queryCase==1)
    			{
    				unsigned long long k=stoull(input);		//string to unsigned long long system function
    				dir.query(timestamp,k);


    			}
    			else
    			{
    				dir.query2(timestamp,input);
    			}

    		}
    	}
    }
    return 0;
}
