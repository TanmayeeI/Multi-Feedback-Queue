#include <iostream>
using namespace std;

typedef struct node {
    int val; 
    int at;  
    int bt;  
    int ct;  
    int wt; 
    int tat;
    node* next; 
} node;

// Function to create the process list
node* create(node* head, int n, int a[], int b[]) {
    node* temp = NULL;
    for (int i = 0; i < n; i++) {
        node* alt = new node();
        alt->val = i;
        alt->at = a[i];
        alt->bt = b[i];
        alt->ct = alt->tat=alt->wt=0;
        alt->next = NULL;
        if (head == NULL) {
            head = alt;
            temp = alt;
        } else {
            temp->next = alt;
            temp = alt;
        }
    }
    return head;
}

// Function to check if all processes are completed
int check(node* head) {
    node* temp = head;
    while (temp != NULL) {
        if (temp->bt != 0)
            return 0; // At least one process has burst time remaining
        temp = temp->next;
    }
    return 1; // All processes are complete
}

// Enqueue function to add a process to a queue
node* enqueue(node* q, int value) {
    node* newNode = new node();
    newNode->val = value;
    newNode->next = NULL;

    if (q == NULL) {
        return newNode;
    }

    node* temp = q;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;

    return q;
}

// Dequeue function to remove a process from a queue
node* dequeue(node* q, int* value) {
    if (q == NULL) {
        *value = -1;
        return NULL;
    }

    node* temp = q;
    *value = temp->val;
    q = q->next;
    delete temp;

    return q;
}

node* calculate(node* table,int a[])
{
    int i=0;
    node* ptr = table;
    while(ptr!=NULL)
    {
        ptr->ct=a[i];
        ptr->tat = ptr->ct - ptr->at;
        ptr->wt = ptr->tat - ptr->bt;
        ptr = ptr->next;
        i++;
    }
    return table;
}
// Display the processes in each queue
void display_queue(node* q0, node* q1, node* q2) {
    cout << "Queue 1: ";
    node* ptr = q0;
    if (ptr == NULL)
        cout << "EMPTY\n";
    else {
        while (ptr != NULL) {
            cout << "P" << ptr->val << " ";
            ptr = ptr->next;
        }
        cout << "\n";
    }

    cout << "Queue 2: ";
    node* ptr1 = q1;
    if (ptr1 == NULL)
        cout << "EMPTY\n";
    else {
        while (ptr1 != NULL) {
            cout << "P" << ptr1->val << " ";
            ptr1 = ptr1->next;
        }
        cout << "\n";
    }

    cout << "Queue 3: ";
    node* ptr2 = q2;
    if (ptr2 == NULL)
        cout << "EMPTY\n";
    else {
        while (ptr2 != NULL) {
            cout << "P" << ptr2->val << " ";
            ptr2 = ptr2->next;
        }
        cout << "\n";
    }
}
void display_chart(node* head,node* time) {
    node* temp = head;

    cout << "\nGantt Chart:\n";
    // Print top border
    for (int i = 0; i < 20; i++) {
        cout << "------";
    }
    cout << "\n";

    // Print process IDs in each segment
    temp = head;
    while (temp != NULL) {
        cout << "|  P" << temp->val+1 << "  ";
        temp = temp->next;
    }
    cout << "|\n";

    // Print bottom border
        for (int i = 0; i < 20; i++) {
        cout << "------";
    }
    cout<<"\n";
    temp = time;
    while (temp != NULL) {
        cout <<temp->val << "     ";
        temp = temp->next;
    }
    
    cout << "\n";  
}

// Feedback Queue Scheduling Implementation
void feedback_queue(node* root, node* q0, node* q1, node* q2, int t0, int t1,int com[],int n) {
    node* time_list=NULL;
    int c = 0; // Current time
    node* temp = root; // Pointer to iterate through the process list
    node* GN = NULL;
    int end=0,i=0;
    time_list= enqueue(time_list,end);
    int val;
    int f=root->at;
    c=f;
    while (!check(root)) {
        // Handle idle time
        
        if (q0 == NULL && q1 == NULL && q2 == NULL &&temp==NULL ) {
        		c++;
        		continue;
        }
       
               
        while (temp != NULL && temp->at <= c) {
            val = temp->val;
            q0 = enqueue(q0, val);
            temp = temp->next;
        }
        
        if (q0 != NULL) {
            
            q0 = dequeue(q0, &val);

            node* proc = root;
            while (proc != NULL && proc->val != val)
                proc = proc->next;
                
            if (proc->bt > t0) {
                proc->bt -= t0;
                c += t0;
                cout<<"Move Process p"<<val+1<<" from queue-1 to queue-2"<<endl;
                q1 = enqueue(q1, val);
                end=c;
            } else {
                c += proc->bt;
                proc->bt = 0;
                cout << "Process P" << proc->val+1<< " completed its execution\n";
                com[val]=c;
                end=c;
            }
        }
       
        else if (q1 != NULL) {
        
            q1 = dequeue(q1, &val);

            node* proc = root;
            while (proc != NULL && proc->val != val)
                proc = proc->next;

            if (proc->bt > t1) {
                proc->bt -= t1;
                c += t1;
                 cout<<"Move Process p"<<val+1<<" from queue-2 to queue-3"<<endl;
                q2 = enqueue(q2, val);
                end=c;
            } else {
                c += proc->bt;
                proc->bt = 0;
                cout << "Process P" << proc->val+1 << " completed its execution\n";
                com[val]=c;
                end=c;
            }
        }
        
        else if (q2 != NULL) {
            q2 = dequeue(q2, &val);

            node* proc = root;
            while (proc != NULL && proc->val != val)
                proc = proc->next;

            c += proc->bt;
            proc->bt = 0;
            cout << "Process P" << proc->val+1<< " completed its execution\n";
            com[val]=c;
            end=c;
        }
        GN=enqueue(GN,val);
        time_list= enqueue(time_list,end);
        
        display_queue(q0, q1, q2); 
        cout<<"\n";
    }
    
    display_chart(GN,time_list);
}


void display(node* root,int n) {
    cout << "P-No\tAT\tBT\tCT\tTAT\tWT\n";
    float atat=0.0;
    float awt=0.0;
    while (root != NULL) {
        awt+=root->wt;
        atat+=root->tat;
        cout << "P" << root->val << ":\t" << root->at << "\t" << root->bt <<"\t"<< root->ct << "\t"<<root->tat << "\t"<<root->wt << "\n";
        root = root->next;
    }
    cout<<"\n";
    cout<<"Average Turnaround time: "<<atat/n<<"\n";
    cout<<"Average waiting time: "<<awt/n<<"\n";
}


int main() {
    int n;
    cout<<"Enter no.of processes:"<<endl;
    cin>>n;
     int a[n], b[n], com[n];
    int i;
    int t0,t1;
    cout<<"enter Arrival times for "<<n<<"processes  in ascending order:"<<endl;
    for(i=0;i<n;i++)
    {
        cin>>a[i];
    }
     cout<<"enter Burst times for "<<n<<"processes :"<<endl;
    for(i=0;i<n;i++)
    {
        cin>>b[i];
    }
    cout<<endl;
    cout<<"Enter time quantum for queue 1"<<endl;
    cin>>t0;
    cout<<"Enter time quantum for queue 2"<<endl;
    cin>>t1;
    
    node* root = NULL;
    root = create(root, n, a, b);

    node* q0 = NULL; // Queue 0
    node* q1 = NULL; // Queue 1
    node* q2 = NULL; // Queue 2

    feedback_queue(root, q0, q1, q2, t0, t1,com,n);
    
    node* table = NULL;
    table = create(table, n, a, b);
    table=calculate(table,com);
    cout<<"\n";
    display(table,n);
    
    return 0;
}