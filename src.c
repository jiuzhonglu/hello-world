#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct
{
	int c_custkey; 
	char c_mkgsegment[20];
}customer;//customer
typedef struct
{
	int o_orderkey;
	int o_custkey;
	char o_orderdate[10];
}orders;//orders
typedef struct
{
	int l_orderkey;
	double l_extendedprice;
	char l_shipdate[10];
}lineitem; //item_in_orders
typedef struct
{
	int l_orderkey;
	char o_orderdate[10];
	double l_extendedprice;
}select_result;deal_result

customer * read_customer_txt() //read customer.txt
{
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(101*sizeof(customer));
	int i=0;
	char b;
	fp = fopen("customer.txt","r");
	if(NULL==fp)
	{
		printf("cannot open customer.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);

		i++;
	}
	fclose(fp);
	return a;
}
orders * read_orders_txt()//read orders.txt
	int i =0;
	orders * a=NULL;
	a = (orders * )malloc(4001*sizeof(orders));
	char b,c;
	long long d;
	FILE *fp;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("cannot open orders.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;

		i++;
	}
	fclose(fp);
	return a;
}
lineitem * read_lineitem_txt()//read lineitem.txt
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1001*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("cannot open lineitem.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);

		i++;
	}
	fclose(fp);
	return l;
}
select_result * Select(customer * cus,orders * ord,lineitem * item,char * order_date,char * ship_date,char * mktsegment)
{
    int i, j, k;  //for cycle
    int sindex=0;  //index of slct_res
    double tmp; //for sort
    select_result * slct_res = NULL;

    slct_res = (select_result *)malloc(1001*sizeof(select_result));

    //create slct_res
    for (i=0; i<100; i++){
        if (strcmp(cus[i].c_mkgsegment, mktsegment)==0){
            for (j=0; j<4000; j++){
                if (cus[i].c_custkey==ord[j].o_custkey && strcmp(ord[j].o_orderdate, order_date)<0){
                    for (k=0; k<1000; k++){
                        if (ord[j].o_orderkey==item[k].l_orderkey && strcmp(item[k].l_shipdate, ship_date)>0){
                            slct_res[sindex].l_extendedprice = item[k].l_extendedprice;
                            slct_res[sindex].l_orderkey = item[k].l_orderkey;
                            strcpy(slct_res[sindex].o_orderdate, ord[j].o_orderdate);
                            sindex++;
                        }
                    }
                }
            }
        }
    }
	for (i=0; i<sindex-1; i++){
		for (j=i+1; j<sindex; j++){
			if (slct_res[i].l_orderkey==slct_res[j].l_orderkey && strcmp(slct_res[i].o_orderdate, slct_res[j].o_orderdate)==0){
				slct_res[i].l_extendedprice = slct_res[i].l_extendedprice + slct_res[j].l_extendedprice;
				slct_res[j].l_extendedprice = 0;
			}
		}
	}
    for (i=0; i<sindex-1; i++){
        for (j=0; j<sindex-1; j++){
            if (slct_res[j].l_extendedprice < slct_res[j+1].l_extendedprice){
                tmp = slct_res[j+1].l_extendedprice;
                slct_res[j+1].l_extendedprice = slct_res[j].l_extendedprice;
                slct_res[j].l_extendedprice = tmp;
            }
        }
    }
    return slct_res;
}

int change_argv_to_number(char s[])//turn (str) into (int)
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);

		i++;
	}
	return number;
}

int main(int argc,char * argv[ ])
{
	int i,j;
	int num;
	int limit=3;
	char order_date[15];
	char ship_date[10];
	char mktsegment[20];
	select_result *result=NULL;
	customer * cus = NULL;
	orders * ord = NULL;
	lineitem * item = NULL;
	cus = read_customer_txt();
	ord = read_orders_txt();
	item = read_lineitem_txt();
	num = change_argv_to_number(argv[4]);
	for(i=0;num>0;num--,i=i+4)
	{
		strcpy(mktsegment,argv[5+i]);
		strcpy(ship_date,argv[7+i]);
		strcpy(order_date,argv[6+i]);
		limit = change_argv_to_number(argv[8+i]);
		result=Select(cus,ord,item,order_date,ship_date,mktsegment);
		printf("l_orderkey|o_orderdate|revenue\n");
		for(j=0;j<limit;j++)
		{
			if(result[j].l_extendedprice==0)
				printf("null      |null       |null   \n");
			else
				printf("%-10d|%-11s|%-20.2lf\n",result[j].l_orderkey,result[j].o_orderdate,result[j].l_extendedprice);
		}
		free(result);
		result = NULL;
	}
	return 0;
}
