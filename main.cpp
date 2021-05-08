#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<windows.h>
#include<mysql.h>
#include <winsock.h>
#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<string>
#include<cstring>
#include<cstdlib>
#include<sstream> 
#include<fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"libmysql.lib")

MYSQL mysql;
MYSQL_RES* result;
MYSQL_ROW row;
char ch[2];

void land();
void showmenu();
void inquire();
int Connectdb();

void search_book();
void add_book();
void borrow_book();
void return_book();
void manage_card();

int main()
{
	int n;
	Connectdb();
	land();
	//showmenu();
	cin >> n;
	while (n)
	{
		switch (n)
		{
		case 1:
			search_book();
			break;
		case 2:
			add_book();
			break;
		case 3:
			borrow_book();
			break;
		case 4:
			return_book();
			break;
		case 5:
			manage_card();
			break;
		case 6:
			exit(0);
		default:break;
		}
		cin >> n;
	}
	mysql_close(&mysql);
	return 0;
}

void land() {
	string aid;
	string password;
	bool pass = false;
	mysql_query(&mysql, "select *from admin");
	result = mysql_store_result(&mysql);

	cout << "\t|-------------------------------------------|" << endl;
	cout << "\t|       Welcome To Book Manage System       |" << endl;
	cout << "\t|-------------------------------------------|" << endl;
	cout << "\t\tplease enter admin id: ";
	cin >> aid;
	cout << "\t\tplease enter password: ";
	cin >> password;
	while (row = mysql_fetch_row(result)) {
		string s1, s2;
		s1 = row[0];
		s2 = row[1];
		if (s1 == aid && s2 == password)
			pass = true;
	}
	if (pass) {
		showmenu();
	}
	else {
		cout << "password error!!!" << endl << "Y : retry" << endl << "N : exit";
	    string a;
	    cin >> a;
		system("cls");
		if (a=="Y" || a == "y")
			land();
		else if (a == "N" || a == "n")
			exit(0);
		else cout << "invalid input!";
	}
}
void showmenu()
{
	system("cls");
	//cout<<"\n\n\n\n\n";
	cout<<"\t|-------------------------------------------|" << endl;
	cout<<"\t|       Welcome To Learn Books System       |" << endl;
	cout<<"\t|-------------------------------------------|" << endl;
	cout<<"\t| 1-search book                             |" << endl;
	cout<<"\t| 2-add book                                |" << endl;
	cout<<"\t| 3-borrow book                             |" << endl;
	cout<<"\t| 4-return book                             |" << endl;
	cout<<"\t| 5-manage card                             |" << endl;
	cout<<"\t|  6-exit                                   |" << endl;
	cout<<"\t|-------------------------------------------|" << endl;
	cout << "\t        ENTER YOUR CHOICE(1-6):";
}
void inquire()
{
	cout << "\tshow the main menu?(y/n):";
	cin >> ch;
	if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
	{
		showmenu();
	}
	else
	{
		exit(0);
	}
}
//连接数据库
int Connectdb()
{
	if (NULL != mysql_init(&mysql)) {
		//cout << "mysql_init() succeed" << endl;
	}
	else {
		cout << "mysql_init() failed" << endl;
		return 0;
	}

	if (0 == mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk")) {
		//cout << "mysql_options() succeed" << endl;
	}
	else {
		cout << "mysql_options() failed" << endl;
		return 0;
	}
	if (!mysql_real_connect(&mysql, "localhost", "root", "2002423aaa", "book_manage", 3306, NULL, 0))
	{
		// cout<<"\n\t Can not connect bookmanager" << endl;
		return 0;
	}
	else
	{
		// cout<<"成功" << endl;
		return 1;
	}
}

//查询图书
void search_book() {
	string input[6], where[6];
	string year[2], price[2];
	string dest;
	string sql;
	if (!Connectdb())
	{
		cout << "\n\t Can not Connectdb bookmanager" << endl;
	}
	else
	{
		system("cls");
		cout << "\t-----------------------------------------" << endl;
		cout << "\t              Search Books               " << endl;
		cout << "\t-----------------------------------------" << endl;
		cout << "\t请输入待查询的图书信息：" << endl;
		cout << "\t缺省值请输入null" << endl;

		cout << "\ncategory:";
		cin >> input[0];
		where[0] = "category='" + input[0] + "'";
		cout << "\nname:";
		cin >> input[1];
		where[1] = "name='" + input[1] + "'";
		cout << "\npublisher:";
		cin >> input[2];
		where[2] = "publisher='" + input[2] + "'";
		cout << "\nyear(区间，以,分隔）:";
		cin >> input[3];
		if (input[3] != "null") {
			stringstream ss(input[3]);
			getline(ss, year[0], ',');
			getline(ss, year[1], ',');
			where[3] = "year between " + year[0] + " and " + year[1];
		}
		cout << "\nauthor:";
		cin >> input[4];
		where[4] = "author='" + input[4] + "'";
		cout << "\nprice(区间，以,分隔）:";
		cin >> input[5];
		if (input[5] != "null") {
			stringstream ss1(input[5]);
			getline(ss1, price[0], ',');
			getline(ss1, price[1], ',');
			where[5] = "price between " + price[0] + " and " + price[1];
		}
		sql = "select *from book where ";
		dest = sql;
		int count = 0;
		for (int i = 0; i < 6; i++)
		{
			if (input[i] != "null") {
				if (count > 0) {
					dest = dest + " and ";
				}
				dest = dest + where[i];
				count++;
			}
		}
		cout << "按照属性排序：" << endl;
		cout << "1:类别" << endl;
		cout << "2:书名" << endl;
		cout << "3:出版社" << endl;
		cout << "4:年份" << endl;
		cout << "5:作者" << endl;
		cout << "6:价格" << endl;
		int i;
		cin >> i;
		switch (i) {
		case 1:
			dest = dest + " ORDER BY " + "category";
			break;
		case 2:
			dest = dest + " ORDER BY " + "name";
			break;
		case 3:
			dest = dest + " ORDER BY " + "publisher";
			break;
		case 4:
			dest = dest + " ORDER BY " + "year";
			break;
		case 5:
			dest = dest + " ORDER BY " + "author";
			break;
		case 6:
			dest = dest + " ORDER BY " + "price";
			break;
		default:break;
		}
		dest = dest + " LIMIT 50";
		if (mysql_query(&mysql, dest.c_str()) != 0)
		{
			cout << "failed";
		}
		else
		{
			cout << "sucess" << endl;
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) == 0)
				cout << "未查询到相关结果!" << endl;
			else{
				system("cls");
				cout << "| bid  | category | name      | publisher      | year | author | price | number | stock |" << endl;
				for (int i = 0; i < mysql_num_rows(result); i++)
				{
					row = mysql_fetch_row(result);
					printf("| % -6s % -10s % -11s % -16s % -6s % -8s % -7s % -8s % -6s|\n", row[0], row[1], row[2], row[3],row[4], row[5], row[6], row[7], row[8]);
				}
			}
		}
		mysql_free_result(result);
	}
	mysql_close(&mysql);
	inquire();
}

//添加图书
void add_book()
{ 
	string category, name, publisher, author, price, bid, year, number;
	string dest;
	string sql;
	if (!Connectdb())
	{
		cout<<"\n\t Can not connect bookmanager" << endl;
	}
	else
	{
		system("cls");
		cout<<"\t-----------------------------------------" << endl;
		cout<<"\t               ADD Books                 " << endl;
		cout<<"\t-----------------------------------------" << endl;
		cout <<"\t             1:单本入库" << endl;
		cout <<"\t             2:批量入库" << endl;
		int i;
		cin >> i;
		if (i == 1)
		{
			cout << "ID:";
			cin >> bid;
			sql = "select *from book where bid=";
			dest = sql + bid;
			if (mysql_query(&mysql, dest.c_str()))
			{
				cout << endl << " the query failed!" << endl;
			}
			else
			{
				result = mysql_store_result(&mysql);
				//row=mysql_fetch_row(result);
				if (mysql_num_rows(result) != 0)
				{
					int num, new_num, new_stock;
					cout << "\n The ID has existe!" << endl;
					cout << "the book number:" << endl;
					row = mysql_fetch_row(result);
					cin >> num;
					new_num = atoi(row[7]) + num;
					new_stock = atoi(row[8]) + num;

					string dest2;
					sql = "update book set number=";
					dest2 = sql + to_string(new_num)+ ",stock="+ to_string(new_stock)+ " where bid=" + bid;
					//cout << dest2;
					if (mysql_query(&mysql, dest2.c_str()) != 0)
					{
						cout << "failed";
					}
					else
					{
						cout << "sucess" << endl;
					}
				}
				else
				{     //mysql_free_result(result);
					  //mysql_close(&mysql);
					cout << "\ncategory:";
					cin >> category;
					cout << "\nname:";
					cin >> name;
					cout << "\npublisher:";
					cin >> publisher;
					cout << "\nyear:";
					cin >> year;
					cout << "\nauthor:";
					cin >> author;
					cout << "\nprice:";
					cin >> price;
					cout << "\nnumber:";
					cin >> number;
					string dest1;
					sql = "insert into book(bid, category, name, publisher, year, author, price, number, stock) values(";
					dest1 = sql + bid;

					dest1 = dest1 + ",'" + category + "','" + name + "','"+ publisher+ "',"+ year+ ",'"+ author+ "',"+ price+ ","+ number+ ","+ number+ ")";

					if (mysql_query(&mysql, dest1.c_str()) != 0)
					{
						cout << "failed";
					}
					else
					{
						cout << "新增成功" << endl;
					}
				}
				mysql_free_result(result);
			}
		}
		else if (i == 2) {
			ifstream in("book.txt");
			string line;
			while (getline(in, line))
			{
				stringstream ss(line);
				string tmp;
				int i = 0;
				string input[8];
				while (getline(ss, tmp, ',')){
					input[i] = tmp;
					i++;
				}
				sql = "insert into book(bid, category, name, publisher, year, author, price, number, stock) values(";
				sql = sql+input[0]+ ",'" + input[1] + "','" + input[2] + "','" + input[3] + "'," + input[4] + ",'" + input[5] + "'," + input[6] + "," + input[7] + "," + input[7] + ")";
				//cout << sql;
				if (mysql_query(&mysql, sql.c_str()))
				{
					cout << endl << " the query failed!" << endl;
				}
				else
				{
					cout << "新增"<<input[1]<<"成功" << endl;
				}
			}
		}
	}
	mysql_close(&mysql);
	inquire();
}

//借书
void borrow_book() {
	string cid, bid,b_time,r_time, aid;
	string sql, sql1,sql2;
	MYSQL_RES *result1,*result2;
	MYSQL_ROW col;

	if (!Connectdb())
	{
		cout << "\n\t Can not connect bookmanager" << endl;
	}
	else
	{
		system("cls");
		cout << "请输入借书证号：" << endl;
		cin >> cid;
		sql = "select *from borrow where cid=" + cid;
		if (mysql_query(&mysql, sql.c_str()))
		{
			cout << endl << " the query failed!" << endl;
		}
		else
		{
			result = mysql_store_result(&mysql);
			//row=mysql_fetch_row(result);
			if (mysql_num_rows(result) == 0) {
				cout << "没有借书记录" << endl;
			}
			else
			{
				system("cls");
				cout << "| bid  | category | name      | publisher      | year | author | price | number | stock |" << endl;
				while (row = mysql_fetch_row(result)) {
					bid = row[0];
					sql1 = "select *from book where bid=" + bid;
					mysql_query(&mysql, sql1.c_str());
					result1 = mysql_store_result(&mysql);
					col = mysql_fetch_row(result1);
					printf("| % -6s % -10s % -11s % -16s % -6s % -8s % -7s % -8s % -6s|\n", col[0], col[1], col[2], col[3], col[4], col[5], col[6], col[7], col[8]);
					mysql_free_result(result1);
				}
			}
			mysql_free_result(result);
		}
		cout << "请输入借阅图书编号：" << endl;
		cin >> bid;
		sql = "select *from book where bid="+bid;
		if (mysql_query(&mysql, sql.c_str()))
		{
			cout << endl << " the query failed!" << endl;
		}
		else
		{
			result = mysql_store_result(&mysql);
			row = mysql_fetch_row(result);
			if (mysql_num_rows(result) == 0)
				cout << "未找到该图书" << endl;
			else if (atoi(row[8]) > 0)
			{   //stock>0
				cout << "请输入借书日期:";
				cin >> b_time;
				cout << "请输入还书日期:" ;
				cin >> r_time;
				cout << "请输入管理员id:";
				cin >> aid;
				//cout << " 借书成功" << endl;
				sql1 = "insert into borrow (bid,cid,b_time,r_time,aid) values(" + bid + "," + cid + ",'" + b_time + "','" + r_time + "'," + aid + ")";
				//cout << sql1;
				if (mysql_query(&mysql, sql1.c_str()))
				{
					cout << endl << " the query failed!" << endl;
				}
				else
				{
					cout << "借书成功" << endl;
					sql2 = "select *from book where bid=" + bid;
					mysql_query(&mysql, sql2.c_str());
					result2 = mysql_store_result(&mysql);
					col = mysql_fetch_row(result2);
					mysql_free_result(result2);

					int new_stock = atoi(col[8]) - 1;
					sql1 = "update book set stock=" + to_string(new_stock) + " where bid=" + bid;
					mysql_query(&mysql, sql1.c_str());
				}
			}
			else 
			{
				cout << "库存不足！最近的归还时间为：";
				sql1 = "select *from borrow where bid=" + bid + " order by r_time ";
				//cout << sql1;
				mysql_query(&mysql, sql1.c_str());
				result1=mysql_store_result(&mysql);
				col = mysql_fetch_row(result1);
				cout << col[3] << endl;
				mysql_free_result(result1);
			}
		}
		mysql_free_result(result);
	}
	mysql_close(&mysql);
	inquire();
}

//还书
void return_book() {
	string cid, bid, b_time, r_time, aid;
	string sql, sql1, sql2;
	MYSQL_RES* result1, * result2;
	MYSQL_ROW col;

	if (!Connectdb())
	{
		cout << "\n\t Can not connect bookmanager" << endl;
	}
	else
	{
		system("cls");
		cout << "请输入借书证号：" << endl;
		cin >> cid;
		sql = "select *from borrow where cid=" + cid;
		if (mysql_query(&mysql, sql.c_str()))
		{
			cout << endl << " the query failed!" << endl;
		}
		else
		{
			result = mysql_store_result(&mysql);
			//row=mysql_fetch_row(result);
			if (mysql_num_rows(result) == 0) {
				cout << "没有借书记录" << endl;
			}
			else
			{
				system("cls");
				cout << "| bid  | category | name      | publisher      | year | author | price | number | stock |" << endl;
				while (row = mysql_fetch_row(result)) {
					bid = row[0];
					sql1 = "select *from book where bid=" + bid;
					mysql_query(&mysql, sql1.c_str());
					result1 = mysql_store_result(&mysql);
					col = mysql_fetch_row(result1);
					printf("| % -6s % -10s % -11s % -16s % -6s % -8s % -7s % -8s % -6s|\n", col[0], col[1], col[2], col[3], col[4], col[5], col[6], col[7], col[8]);
					mysql_free_result(result1);
				}
			}
			mysql_free_result(result);
		}
		cout << "请输入归还图书编号：" << endl;
		cin >> bid;
		sql = "select *from borrow where bid=" + bid;
		if (mysql_query(&mysql, sql.c_str()))
		{
			cout << endl << " the query failed!" << endl;
		}
		else
		{
			result = mysql_store_result(&mysql);
			//row=mysql_fetch_row(result);
			if (mysql_num_rows(result) != 0)
			{
				sql1 = "delete from borrow where bid=" + bid + " and cid=" + cid;
				mysql_query(&mysql, sql1.c_str());

				sql2 = "select stock from book where bid=" + bid;
				mysql_query(&mysql, sql2.c_str());
				result1 = mysql_store_result(&mysql);
				row = mysql_fetch_row(result1);
				int new_stock = atoi(row[0]) + 1;

				sql1 = "update book set stock=" + to_string(new_stock) + " where bid=" + bid;
				mysql_query(&mysql, sql1.c_str());

				cout << "还书成功！" << endl;
				mysql_free_result(result1);
			}
			else cout << "还书失败，未找到借阅信息！" << endl;
		}
		mysql_free_result(result);
	}
	mysql_close(&mysql);
	inquire();
}

//借书证管理
void manage_card() {
	{
		string cid, name, company, identity;
		string dest;
		string sql;
		if (!Connectdb())
		{
			cout << "\n\t Can not connect bookmanager" << endl;
		}
		else
		{
			system("cls");
			cout << "\t-----------------------------------------" << endl;
			cout << "\t               Manage Cards              " << endl;
			cout << "\t-----------------------------------------" << endl;
			cout << "\t             1:新增借书证" << endl;
			cout << "\t             2:删除借书证" << endl;
			int i;
			cin >> i;
			if (i == 1)
			{
				cout << "cid:";
				cin >> cid;
				sql = "select *from card where cid=";
				dest = sql + cid;
				if (mysql_query(&mysql, dest.c_str()))
				{
					cout << endl << " the query failed!" << endl;
				}
				else
				{
					result = mysql_store_result(&mysql);
					//row=mysql_fetch_row(result);
					if (mysql_num_rows(result) != 0)
					{
						cout << "\n The card ID has exist!" << endl;
					}
					else
					{     //mysql_free_result(result);
						  //mysql_close(&mysql);
						//cout << "\ncid:";
						//cin >> cid;
						cout << "\nname:";
						cin >> name;
						cout << "\ncompany:";
						cin >> company;
						cout << "\nidentity:";
						cin >> identity;

						string dest1;
						sql = "insert into card(cid,name,company,identity) values(";
						dest1 = sql + cid;
						dest1 = dest1 + ",'" + name + "','" + company + "','" + identity + "')";

						if (mysql_query(&mysql, dest1.c_str()) != 0)
						{
							cout << "failed";
						}
						else
						{
							cout << "新增成功！" << endl;
						}
					}
					mysql_free_result(result);
				}
			}
			else if (i == 2) {
				cout << "请输入要删除的借书证号：";
				cin >> cid;
				sql = "delete from card where cid=" + cid;
				if (mysql_query(&mysql, sql.c_str()) != 0)
				{
					cout << "failed";
				}
				else
				{
					cout << "删除成功！" << endl;
				}
			}
		}
		mysql_close(&mysql);
		inquire();
	}
}