#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <fstream>


using namespace std ;

vector <string> foods = { "Beef Stew " , "Pasta with Tomato Sauce" , "Vegetable Stir-Fry" , "Pizza " , "Fish Fillet" , "Chocolate Cake" , "Apple Pie" , "Coffee" , "Soda" , "Lemonade" } ;



struct Order {
	string	firstName ;
	string	 lastName ;
	string	studentID ;
	int		   status ; // 0 = canceled , 1 = ordered , 2 = delivered
	string		order ;
	Order*		 next ;
};

void addOrder(Order*& head, string name, string family, string id, string orders) {
    Order* newOrder = new Order();

    newOrder->firstName = name;
    newOrder->lastName = family;
    newOrder->studentID = id;
    newOrder->order = orders;
    newOrder->status = 1; // ordered status

    newOrder->next = nullptr;

    if (head == nullptr) {
        head = newOrder;
    } else {
        Order* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newOrder;
    }
}

Order* found(Order* head, string orderID) {
    int index = stoi(orderID) - 1;

    Order* current = head;
    for (int i = 0; i < index; i++) {
        if (current == nullptr)
            return nullptr;
        current = current->next;
    }

    return current;
}



void delivered(Order** head, string orderID) {
    Order* order = found(*head, orderID);

    if (!order) { // order isn't found !!
        cout << "Order isn't found !!" << endl;
        return;
    }

    cout << "Delivered" << endl;
    order->status = 2; // 2 = delivered status
}

void canceled(Order** head, string orderID) {
    Order* order = found(*head, orderID);

    if (!order) { // order isn't found !!
        cout << "Order isn't found !!" << endl;
        return;
    }

    cout << "Canceled" << endl;
    order->status = 0; // 0 = canceled status
}

void writeToFile(Order* head ) {
    ofstream outputFile("orders.txt");

    if (outputFile.is_open()) {
        Order* current = head;
        int index = 1 ;
        while (current != nullptr) {
            outputFile << index << "," << current->firstName  << "," << current->lastName << "," << current->studentID << "," <<
            	current->status << "," << current->order << endl ;
            	
            current = current->next;
            index ++ ;
        }

        outputFile.close();
    } else {
        cout << "Unable to open the file." << endl;
    }
}

void readOrdersFromFile(Order*& head) {
    ifstream inputFile("orders.txt");

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            Order* newOrder = new Order;

            size_t pos = 0;
            string token;

            pos = line.find(",");
            line.erase(0, pos + 1);
            
            pos = line.find(",");
            newOrder->firstName = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            newOrder->lastName = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            newOrder->studentID = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            newOrder->status = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(",");
            newOrder->order = line.substr(0, pos);
            line.erase(0, pos + 1);

            newOrder->next = nullptr;

            if (head == nullptr) {
                head = newOrder;
            } else {
                Order* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newOrder;
            }
        }

        inputFile.close();
    } else {
        cout << "Unable to open the file." << endl;
    }
}


int main(){
	
	Order* head = nullptr ;
	readOrdersFromFile(head);

	
	while (true){
		
		cout << "*******************\n|    Food Self    |\n*******************\n 1    Menu\n 2    New order\n 3    Order delivered\n 4    Cancel order\n"
			" 5    Change order\n 6    Review\n\n 0    Exit\n\nEnter your choise :  " ;
		string choiseEntry ;
		cin >> choiseEntry ; 
		
		if (choiseEntry == "1" ){ // Menu
		
			cout << "Menu items :\n" ;
			for (string food : foods){
				cout << "  " << food << endl << endl  ;
			}			
			
		} else if (choiseEntry == "2" ){ // New order 
		
			string name , family , id , foods ;
			
			cout << "Enter your first-name : " ;
			cin >> name ;
			
			cout << "Enter your last-name : " ;
			cin >> family ;

			cout << "Enter your student-id : " ;
			cin >> id ;
			
			cin.ignore(); // for read next line (foods)
			cout << "Enter your foods : " ;
			getline(cin, foods);

			cout << "are you sure ? y/n " ;
			string accept ;
			cin >> accept ;
			if (accept == "y"){
				addOrder(head , name , family , id , foods) ;
				
				cout << "Order submit!\n" << endl;
			} else {
				cout << "Canceled!\n" << endl;
			}
						
		} else if (choiseEntry == "3" ){ // Order delivered
			
			cout << "Enter order ID : " ;
			string orderID ;
			cin >> orderID ;
		
			if (found(head , orderID)->status != 0 ){
				delivered(&head , orderID) ;
				cout << endl;
			} else {
				cout << "Error!! Order canceled!!" << endl ;
			}
			
		} else if (choiseEntry == "4" ){ // Cancel order

			cout << "Enter order ID : " ;		
			string orderID ;
			cin >> orderID ;
			
			if (found(head , orderID)->status != 2 ){
				canceled(&head , orderID) ;
				cout << endl;
			} else {
				cout << "Error!! Order delivered!!" << endl ;
			}
			
				
		} else if (choiseEntry == "5" ){ // Change order
		
			cout << "Enter order ID : " ;		
			string orderID ;
			cin >> orderID ;
			
			Order* order = found(head , orderID);
			
			if (order){
				
		        string status;
		        if (order->status == 1)
		            status = "Submit";
		        else if (order->status == 2)
		            status = "Delivered";
		        else if (order->status == 0)
		            status = "Canceled";
		            
				cout << "Name : " << order->firstName << " " << order->lastName << endl ;
				cout << "Student ID : " << order->studentID << endl ;
				cout << "Status : " << status << endl ;
				cout << "Order : " << order->order << endl << endl ; 
				
				if (order->status != 1) 
					cout << "You can't change this order ; Order is " << status << endl ;
				else {
					string newOrder ;
					cout << "Enter new foods : " ;
					cin.ignore(); 
					getline(cin, newOrder);
					
					if (newOrder == "" ) cout << "Order shouldn't be empty!" << endl ;
					else {
						order->order = newOrder ;
						cout << "OK!" << endl ;
					}
					
				}
			} else {
				cout << "Order isn't found!!" << endl;
			}
		} else if (choiseEntry == "6" ){ // Review
			
			cout << endl << setw(10) << left << "Order ID" << setw(40) << left << "Name" << setw(15) << left << "Student ID" << setw(15) << left << "Status" << "Orders" << endl;
			cout << "________________________________________________________________________________________\n";
			
			if (head){
				int index = 1;
			    
			    Order* current = head;
			    while (current) {
			        string status;
			        if (current->status == 1)
			            status = "Submit";
			        else if (current->status == 2)
			            status = "Delivered";
			        else if (current->status == 0)
			            status = "Canceled";
			
			        cout << setw(10) << left << index << setw(40) << left << (current->firstName + " " + current->lastName) << setw(15) << left << current->studentID << setw(15) << left << status << current->order << endl;
			
			        current = current->next;
			        index++;
			    }
				
				
			} else { // no order
				cout <<"				Empty!\n" ;	
			}
			
			cout << "________________________________________________________________________________________\n"<< endl;
				
		} else if (choiseEntry == "0" ){ // Exit
			break ; // close 
		} else {
			cout << "\nEnter the correct item\nEnter any chracter to continue " ;
			char temp ;
			cin >> temp ; 
			system("cls"); // clear terminal 
		}
		writeToFile(head);
	}
	
	
	return 0 ;
}