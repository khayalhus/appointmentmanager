/* @Author
Student Name: Khayal Huseynov
Student ID : 150180903
Date: 13/11/2019 */
/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	//THIS FUNCTION WILL BE CODED BY YOU
	head = NULL;
}

void WorkPlan::close()
{
	//THIS FUNCTION WILL BE CODED BY YOU
	Task* traverse = head;
	Task* tail;
	head->previous->next = NULL; // break the circularity
	while (head != NULL) {
		head = head->next;
		while (traverse != NULL) {
			tail = traverse;
			traverse = traverse->counterpart;
			delete tail; // delete the whole day starting from the first task of the day
		}
		traverse = head;
	}
}

void WorkPlan::add(Task *task)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	Task *newTask = new Task(); // create the task to be added
	newTask->name = new char [strlen(task->name)]; // allocate memory for name array seperately
	// copy elements to newly created task
	strcpy(newTask->name, task->name);
	newTask->priority = task->priority;
	newTask->day = task->day;
	newTask->time = task->time;

	if (head == NULL) {
		head = newTask;
		head->next = head;
		head->previous = head;
		head->counterpart = NULL;
		return; // add as first node (starting day, first task)
	} 
	
	Task* traverse = head;
	do {
		if (newTask->day == traverse->day) {
			Task* compeer = traverse;
			Task* tail;
			while (compeer != NULL) {
				if (newTask->time == compeer->time) {
					addToSameTime(newTask, compeer); // delays one of them depending on the priorities
					return; // add to same time
				} else if (newTask->time < compeer->time) {
					if (compeer->next != NULL || compeer->previous != NULL) {
						if (compeer == head) {
							head = newTask; // add as the first task of the first day
						}
						newTask->next = compeer->next;
						newTask->previous = compeer->previous;
						compeer->previous->next = newTask;
						compeer->next->previous = newTask;
						compeer->next = NULL;
						compeer->previous = NULL;
						newTask->counterpart = compeer;
						return; // add before first task of a day
					}
					else {
						newTask->next = NULL;
						newTask->previous = NULL;
						tail->counterpart = newTask;
						newTask->counterpart = compeer;
						return; // add in between tasks of a day
					}
				}
				tail = compeer;
				compeer = compeer->counterpart;
			}
			newTask->next = NULL;
			newTask->previous = NULL;
			newTask->counterpart = NULL;
			tail->counterpart = newTask; // add as the last task of the day
			return; // add task to an existing day
		}
		else if (newTask->day < traverse->day) {
			(traverse->previous)->next = newTask;
			newTask->previous = traverse->previous;
			newTask->next = traverse;
			traverse->previous = newTask;
			newTask->counterpart = NULL;
			if (traverse == head) {
				head = newTask; // add before the starting day
			}
			return; // add in between days
		}
		traverse = traverse->next;
	} while (traverse != head);

	(traverse->previous)->next = newTask;
	newTask->previous = traverse->previous;
	newTask->next = traverse;
	traverse->previous = newTask;
	newTask->counterpart = NULL;
	return;
	// add after the last day (tail)
} 

void WorkPlan::addToSameTime(Task *newTask, Task *compeer) {
	//THIS FUNCTION WAS ADDED BY ME TO IMPROVE READABILITY
	if (newTask->priority > compeer->priority) {
		// add task in place of compeer
		if (compeer->next != NULL || compeer->previous != NULL) {
			if (compeer == head) {
				head = newTask;	// add before the first task of the starting day
			}
			newTask->next = compeer->next;
			newTask->previous = compeer->previous;
			compeer->previous->next = newTask;
			compeer->next->previous = newTask;
			compeer->next = NULL;
			compeer->previous = NULL;
			newTask->counterpart = compeer->counterpart;
			compeer->counterpart = NULL;
			// add before first task of a day that is not the starting day
		}
		// delay compeer to another time
		checkAvailableNextTimesFor(compeer);
		compeer->day = usable_day;
		compeer->time = usable_time;
		add(compeer); // add compeer to next available or not allocated time
	} else {
		// delay task to another time
		checkAvailableNextTimesFor(newTask);
		newTask->day = usable_day;
		newTask->time = usable_time;
		add(newTask); // add task to next available or not allocated time
	}
}

Task * WorkPlan::getTask(int day, int time)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	Task* traverse = head;
	do {
		if (traverse->day == day) {
			while (traverse != NULL && traverse->time != time) {
				traverse = traverse->counterpart;
			}
			if (traverse->time == time) {
				return traverse;
			}
			else {
				return NULL;
			}
		}
		traverse = traverse->next;
	} while (traverse != head);
	return NULL;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	//THIS FUNCTION WILL BE CODED BY YOU
	Task* traverse = head;
	Task* compeer;
	traverse = head;
	while (delayed->day != traverse->day) {
		traverse = traverse->next;
		if (traverse == head) break;
	}
	compeer = traverse;
	int time = delayed->time;
	if (delayed->time != 16) {
		// check available next time on the same day
		while (compeer->counterpart != NULL && delayed->time != compeer->time) {
			compeer = compeer->counterpart;
		}
		while (compeer->counterpart != NULL) {
		// check if there is an empty hour between tasks
			if (time + 1 != compeer->counterpart->time) {
				// if there is assign usable_day and usable_time accordingly
				usable_day = compeer->day;
				usable_time = time + 1;
				return;
			}
			time++; //same as time = compeer->counterpart->time
			compeer = compeer->counterpart;
		}
	}
	if (traverse->next == head) {
		checkNextNotAllocatedTimesFor(delayed);
		return;
	}
	// check for available next time on the following days
	// used for delaying a single task and a whole day
	compeer = traverse->next;
	time = 7;
	do {
		do {
			if (time + 1 != compeer->time) {
				usable_day = compeer->day;
				usable_time = time + 1;
				return;
			}
			time++;
			compeer = compeer->counterpart;
		} while (compeer != NULL);
		traverse = traverse->next;
		time = 7;
		compeer = traverse;
	} while (compeer != head); // for delaying all tasks in a day
	checkNextNotAllocatedTimesFor(delayed); // check for next not allocated time if there is no available time
	return;
}

void WorkPlan::checkNextNotAllocatedTimesFor(Task *delayed) {
	Task* traverse = head; // to remember the day the task is in
	Task* compeer; // to traverse the tasks of a day
	while (delayed->day != traverse->day) {
		traverse = traverse->next;
		if (traverse == head) break;
	}
	compeer = traverse;
	if (delayed->time != 16) {
		// check for next not allocated time on the same day
		while (compeer->counterpart != NULL) {
			compeer = compeer->counterpart;
		}
		if (compeer->time + 1 != 16) {
			usable_day = compeer->day;
			usable_time = compeer->time + 1;
			return;
		}
	}

	if (traverse->next == head) {
		usable_day = traverse->day + 1;
		usable_time = 8;
		return; // make new day after last day and task at 8am
	}

	// check for next not allocated time on the following days
	traverse = traverse->next;
	compeer = traverse;
	do {
		while (compeer->counterpart != NULL) {
			compeer = compeer->counterpart;
		}
		// check if task can be added after one hour from the last task (and if that would be in working hours)
		if (compeer->time + 1 != 16) {
			usable_day = compeer->day;
			usable_time = compeer->time + 1;
			return;
		}
		traverse = traverse->next;
		compeer = traverse;
	} while (compeer != head); // for delaying all tasks in a day

	usable_day = head->previous->day + 1;
	usable_time = 8;
	return; // create new day after last day and task at 8am
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	Task* traverse = head;
	Task* tail;
	while (traverse->day != day) {
		traverse = traverse->next;
		if (traverse == head) return;
	}
	// delay tasks while respecting their place on the day
	while (traverse != NULL) {
		// this method replaces the task so it is necessary to allocate new memory
		Task *delayed = new Task();
		delayed->name = new char [strlen(traverse->name)];
		// copy elements to newly created task
		strcpy(delayed->name, traverse->name);
		delayed->priority = traverse->priority;
		// this is to start checking from the next day (in order to seperate from delaying a single task)
		delayed->day = day;
		delayed->time = 16;
		checkAvailableNextTimesFor(delayed);
		delayed->day = usable_day;
		delayed->time = usable_time;
		tail = traverse;
		traverse = traverse->counterpart;
		remove(tail); // remove from old position
		add(delayed); // add to new position
	}
}

void WorkPlan::remove(Task *target)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	if (target->next != NULL || target->previous != NULL) {
		// remove first task of a day
		if (target->counterpart == NULL) {
			// remove first task of a day with no counterpart
			// remove the whole day
			(target->previous)->next = target->next;
			(target->next)->previous = target->previous;
		} else {
			// remove first task of a day with a counterpart
			// make counterpart the first task
			(target->previous)->next = target->counterpart;
			(target->counterpart)->next = target->next;
			(target->counterpart)->previous = target->previous;
			(target->next)->previous = target->counterpart;
		}
	}
	else {
		Task* traverse = head;
		Task* tail;
		do {
			if (traverse->day == target->day) {
				while (traverse != NULL && traverse->time != target->time) {
					tail = traverse;
					traverse = traverse->counterpart;
				}
				if (traverse->time == target->time) {
					tail->counterpart = traverse->counterpart;
					delete target;
					return;
				}
			}
			traverse = traverse->next;
		} while (traverse != head);
	}
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
