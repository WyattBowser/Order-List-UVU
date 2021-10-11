#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <math.h>
#include "split.h"

using namespace std;




//==================================================================CUSTOMER CLASS =================================================================================================================

class Customer
{
private:
	int cust_id;
	string name;
	string street;
	string city;
	string state;
	string zip;
	string phone;
	string email;

public:
	Customer()
	{
		cust_id = 0;
		name = "N/A";
		street = "N/A";
		city = "N/A";
		state = "N/A";
		zip = "N/A";
		phone = "N/A";
		email = "N/A";
	}

	Customer(string id, vector<Customer>& customerVector)
	{
		//Look through the customer vector
		for (auto currentCustomer : customerVector)
		{
			if (currentCustomer.getID() == id)
			{
				this->cust_id = currentCustomer.cust_id;
				this->name = currentCustomer.name;
				this->street = currentCustomer.street;
				this->city = currentCustomer.city;
				this->state = currentCustomer.state;
				this->zip = currentCustomer.zip;
				this->phone = currentCustomer.phone;
				this->email = currentCustomer.email;

			}
		}
	}

	Customer(string id, string name, string street, string city, string state, string zip, string phone, string email)
	{
		cust_id = stoi(id);
		this->name = name;
		this->street = street;
		this->city = city;
		this->state = state;
		this->zip = zip;
		this->phone = phone;
		this->email = email;
	}

	string getID()
	{
		return to_string(cust_id);
	}

	string print_details()
	{
		return "Customer ID #" + std::to_string(cust_id) + ":" + "\n" + name + ", ph. " + phone + ", email: " + email + "\n" + street + "\n" + city + ", " + state + " " + zip + "\n";
	}


};




//==================================================================ITEM CLASS=======================================================================================================================

class Item
{
private:
	int item_ID;
	string description;
	double price;

public:
	void setData(string id, string description, string price)
	{
		this->item_ID = stoi(id);
		this->description = description;
		this->price = stod(price);
	}

	int getID()
	{
		return item_ID;
	}

	string getDescription()
	{
		return description;
	}

	double getPrice()
	{
		return price;
	}
};




//==================================================================LINE ITEM========================================================================================================================
class lineItem
{
private:
	int item_ID;
	int qty;
	Item item;

public:

	lineItem(string item_ID, string qty, const vector<Item>& itemVector)
	{
		this->item_ID = stoi(item_ID);
		this->qty = stoi(qty);

		string description;
		string price;

		//search for the item
		for (auto currentItem : itemVector)
		{
			//If the item id's match
			if (currentItem.getID() == this->item_ID)
			{
				//Set the item id
				item.setData(item_ID, currentItem.getDescription(), to_string(currentItem.getPrice()));
			}
		}
	}
	//Return subtotal
	double sub_total()
	{
		return qty * item.getPrice();
	}

	string print_details()
	{
		ostringstream collection;
		collection << fixed << setprecision(2) << "Item " << item_ID << ": \"" << item.getDescription() << "\", " << qty << " @ " << item.getPrice() << "\n";
		return collection.str();
	}
};





//==================================================================PAYMENT BASE========================================================================================================================
class Payment
{
protected:
	double amount;
public:
	Payment()
	{
		amount = 0;
	}

	Payment(double amount)
	{
		this->amount = amount;
	}


	virtual string print_details()
	{
		ostringstream output;
		output << fixed << setprecision(2) << amount;
		return output.str();
	}
};

class Credit : public Payment
{
private:
	string card_number;
	string expiration;

public:
	Credit();
	Credit(string number, string exp, double amount) : Payment(amount)
	{
		card_number = number;
		expiration = exp;
	}

	virtual string print_details()
	{

		ostringstream output;
		output << fixed << setprecision(2) << "Amount: $" << amount << ", Paid by Credit card " + card_number + ", exp. " + expiration + "\n";
		return output.str();
	}
};

class Paypal : public Payment
{
private:
	string paypal_id;

public:
	Paypal();
	Paypal(string id, double amount) : Payment(amount)
	{
		paypal_id = id;
	}

	virtual string print_details()
	{
		ostringstream output;
		output << fixed << setprecision(2) << "Amount: $" << amount << ", Paid by Paypal ID: " + paypal_id + "\n";
		return output.str();
	}
};

class WireTransfer : public Payment
{
private:
	string bank_id;
	string account_id;

public:
	WireTransfer();
	WireTransfer(string bank, string account, double amount) : Payment(amount)
	{
		bank_id = bank;
		account_id = account;
	}

	virtual string print_details()
	{
		ostringstream output;
		output << fixed << setprecision(2) << "Amount: $" << amount << ", Paid by Wire transfer from Bank ID " + bank_id + ", Account# " + account_id + "\n";
		return output.str();
	}
};





//===================================================================ORDER CLASS=======================================================================================================================
class Order
{

private:

	Customer customer;
	//MAY HAVE TO DO SOME VIRTUAL SHIT
	Payment payment;
	vector<lineItem> items;

	int order_id;
	string order_date;
	int customer_id;


public:
	Order()
	{}

	Order(Customer customer, Payment payment, string order_id, string order_date, string customer_id, vector<lineItem> items)
	{
		this->customer = customer;
		this->payment = payment;
		this->items = items;
		this->order_id = stoi(order_id);
		this->order_date = order_date;
		this->customer_id = stoi(customer_id);
		this->items = items;
	}


	void addLineItem(lineItem items)
	{
		this->items.push_back(items);
	}

	double total()
	{
		//Make a variable to hold the total
		double total = 0;

		//Go through the item list
		for (auto item : items)
		{
			//Add each subtotal to the total
			total += item.sub_total();
		}

		//return total
		return total;
	}

	string print_order()
	{
		string output = "=======================================================\n"
						"=======================NEW ORDER=======================\n"
						"=======================================================\n"
			"Order #" + to_string(order_id) + " Date: " + order_date + "\n" +
			payment.print_details() + "\n" +
			customer.print_details() + "\n" +
			"Order Details:" + "\n";

			for (auto item : items)
			{
				output += "\t" + item.print_details();
			}

			output += "===========================\n"
				"=======END OF SALE=========\n"
				"===========================\t\t\n\n\n";
			return output;
	}

	
};


//====================================================================GLOBAL VECTORS====================================================================================================================
vector<Customer> customerVector;
vector<Item> itemVector;
vector<Order> orders;


//===================================================================READ FILE FUNCTIONS ===============================================================================================================
void read_customers(string fileName)
{
	string line;
	//Make a new file input
	ifstream fileInput;

	//Open the file
	fileInput.open(fileName);

	//While fileInput is valid
	while (fileInput)
	{
		//Grab the next line and save it
		getline(fileInput, line);

		//Parse the data
		auto parsedLine = split(line, ',');

		//Make sure that the parsed line has data
		if (parsedLine.size() != 0)
		{
			//Put the data into a customer object
			Customer newCustomer(parsedLine.at(0), parsedLine.at(1), parsedLine.at(2), parsedLine.at(3), parsedLine.at(4), parsedLine.at(5), parsedLine.at(6), parsedLine.at(7));

			//Save the customer into the customer vector
			customerVector.push_back(newCustomer);
		}
	}
}

void read_items(string fileName)
{
	ifstream fileInput;

	//Open the file
	fileInput.open(fileName);

	//While fileInput is valid
	while (fileInput)
	{
		string line;

		//Grab the next line and save it
		getline(fileInput, line);
		//Parse the line
		auto parsedLine = split(line, ',');

		//If there is data in the parsed line
		if (parsedLine.size() != 0)
		{
			Item newItem;
			newItem.setData(parsedLine.at(0), parsedLine.at(1), parsedLine.at(2));
			itemVector.push_back(newItem);
		}

	}


}

//UNFINISHED
void read_orders(string fileName)
{
	ifstream fileInput;

	//Open the file
	fileInput.open(fileName);

	//While fileInput is valid
	while (fileInput)
	{

		string firstLine;
		string customer_id;
		string order_number;
		string order_date;
		vector<lineItem> items;



		//Grab the first line and save it
		getline(fileInput, firstLine);
		auto parsedFirstLine = split(firstLine, ',');


		//Start deviding the info where it needs to go
		if (parsedFirstLine.size() != 0)
		{
			//Save customer info for this order
			customer_id = parsedFirstLine.at(0);
			order_number = parsedFirstLine.at(1);
			order_date = parsedFirstLine.at(2);




			//go through the list of items
			for (int i = 3; i < parsedFirstLine.size(); i++)
			{
				//Split up the item/qty pair
				auto itemQtyPair = split(parsedFirstLine.at(i), '-');

				//Make a new Line Item
				lineItem newItems(itemQtyPair.at(0), itemQtyPair.at(1), itemVector);

				//Add it to the items vector for this order
				items.push_back(newItems);
			}
		}

		//Make a customer with the data
		Customer newCustomer(customer_id, customerVector);

		//calculate subtotal
		double totalPrice = 0;

		for (auto currentLineItem : items)
		{
			totalPrice += currentLineItem.sub_total();
		}



		//Take care of the second line
		string secondLine;
		string paymentCode;
		string information;


		//Grab the second line and save it
		getline(fileInput, secondLine);

		//Parse it
		auto parsedSecondLine = split(secondLine, ',');

		Payment newPayment;

		//See what form of payment they did
		if (parsedSecondLine.size() != 0)
		{
			switch (stoi(parsedSecondLine.at(0)))
			{
				//Credit Card
			case 1:
				newPayment = Credit(parsedSecondLine.at(1), parsedSecondLine.at(2), totalPrice);
				break;

				//Paypal
			case 2:
				newPayment = Paypal(parsedSecondLine.at(1), totalPrice);
				break;

				//Wire Transfer
			case 3:
				newPayment = WireTransfer(parsedSecondLine.at(1), parsedSecondLine.at(2), totalPrice);
				break;
			}



			//Create an order
			Order newOrder(newCustomer, newPayment, order_number, order_date, customer_id, items);

			//Throw it in the order vector
			orders.push_back(newOrder);
		}
	}
}



//==================================================================MAIN===========================================================================

int main()
{
	read_customers("customers.txt");
	read_items("items.txt");
	read_orders("orders.txt");


	/*passed
	Customer testCustomer("000" , "Test Name", "Test Street", "test city", "test state", "123", "456", "test Email");
	Paypal testPaypal("Test Paypal ID", 420.69);
	lineItem testLine("47433", "3", itemVector);
	//Credit testCredit("TestNumber", "TestExp", 12.50);
	//cout << testCredit.print_details();
	WireTransfer testWire("Bank", "ID", 420.69);
	testWire.print_details();

	Order testOrder(testCustomer, &testPaypal, "123", "456", "123");
	testOrder.addLineItem(testLine);
	cout << testOrder.print_order();
	*/

	ostringstream finalOutput;
	//New of stream
	ofstream writeFile;
	writeFile.open("orders_list.txt");


	for (auto currentOrder : orders)
	{
		writeFile << currentOrder.print_order();
	}

	writeFile.close();
}