#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using namespace std;
//#include "util.h"

//#include <bsd/stdlib.h>

long  extract_or_fail_date(const char *str);
double extractd_or_fail(const char *str);
long parse_date(const char *date);


// union Data {
//    double f_value ;
// //    long long i_value ;
// } ;  


int N = 100;
// long l_shipdate[100];
// long long  l_discount[100];
// long long l_quantity[6001215][2];
// long long l_extendedprice[100];

typedef enum {
    L_ORDERKEY = 0,
    L_PARTKEY,
    L_SUPPKEY,
    L_LINENUMBER,
    L_QUANTITY,
    L_EXTENDEDPRICE,
    L_DISCOUNT,
    L_TAX,
    L_RETURNFLAG,
    L_LINESTATUS,
    L_SHIPDATE,
    L_COMMITDATE,
    L_RECEIPTDATE,
    L_SHIPINSTRUCT,
    L_SHIPMODE,
    L_COMMENT,
} LineItemKeyNames;

extern "C" int loadData_q6(double l_quantity_discount[100][3], long l_shipdate[100]) {
  int N = 100;
//   long l_shipdate[100];


  FILE *tbl;
  int count;
  
  size_t length = -1;

  long lines = N;
//  std::cout<<"Hello;";
//   fprintf(stderr, "Loading data from tpch-dbgen/lineitem.tbl...");
//   fflush(stderr);
//   auto start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//   std::cout<< "File loading start = ";
  tbl = fopen("/homes/sverma3/lineitem.tbl", "r");

//   tbl = fopen("/opt/tpch_data/TPC-H V3.0.1/dbgen/lineitem.csv", "r");

    const int size = 4096;

    char *buf = (char *)malloc(size);

    int i = 0;
    int ret = -1;
    while (fgets(buf, size, tbl) && lines >= 0) {
        char *line = buf;
        char *token;
        int column = 0;

        if (i >= length)
            break;

        while ((token = strsep(&line, "|")) != NULL) {
            double value;
            long datec;
            
            switch (column) {
                case L_SHIPDATE:
                    datec = extract_or_fail_date(token);
                    l_shipdate[i] = datec;
                    // std::cout << l_shipdate[i] << '\n';
                    break;
                case L_QUANTITY:
                    value = extractd_or_fail(token);
                    l_quantity_discount[i][0] = value;
                    break;
                case L_DISCOUNT:
                    value = extractd_or_fail(token);
                    l_quantity_discount[i][1] = value;
                    break;
                case L_EXTENDEDPRICE:
                    value = extractd_or_fail(token);
                    l_quantity_discount[i][2] = value;
                    break;
                default:
                    break;
            }

            column++;
        }

        i++;
        lines--;
    }

    ret = i;
    // const auto p1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    // std::cout << "\n file _ loading duration =   " << p1-start;
    
//   fprintf(stderr, "loaded %d lines!\n", ret);
//   fflush(stderr);
//   double sum =0;
//   fprintf(stderr, "QUANTITY | DISCOUNT | EXTENED_PRICE\n");
//   for(int i=0; i < ret; i++){
//      if(l_quantity_discount[i][0] <= 20 /*&& l_quantity_discount[i][1].f_value >= 0.5 && l_quantity_discount[i][1].f_value <= 0.10*/){
//        fprintf(stderr, "%.2f | %.2f | %.2f <> ", l_quantity_discount[i][0], l_quantity_discount[i][1], l_quantity_discount[i][2]);
//         // fprintf(stderr, "something %d", l_quantity_discount[i][0].f_value);
//         // std::cout << l_quantity_discount[i][0] << ' | ' << l_quantity_discount[i][1] << ' | ' << l_quantity_discount[i][2] << '\n';
//      }
//   }
  
//   fprintf(stderr, "sum =%lf \n", sum);
//   fprintf(stderr,"\n Timestamp: %ld ", duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
//   fflush(stderr);

exit:
    return ret;
}


extern "C" int time_exec(){
  //std::cout << "\n Current Timestamp: " << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  fprintf(stderr,"\n Current Timestamp: %ld ", duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  fflush(stderr);
  return 1;

}


long parse_date(const char *date) {

    char *buf, *tofree;

    tofree = buf = strdup(date);

    char *c;
    char *token;
    long value;
    long result[3];

    int i = 0;
    while ((token = strsep((char **)(&buf), "-")) != NULL) {

        value = strtoul(token, &c, 10);
        if (token == c) {
            fprintf(stderr, "[parse_date]\t couldn't parse date %s\n", date);
            exit(1);
        }

        result[i] = value;
        i++;

        if (i > 3)
            break;
    }

    free(tofree);
    //fprintf(stderr, "date %ld, %ld, %ld\n", result[0], result[1]*100, result[0]*1000);
    return result[2] + result[1] * 100 + result[0] * 10000;
}

long   extract_or_fail_date(const char *str){
    std::tm t = {};
    if(strptime(str, "%Y-%m-%d", &t) == NULL){
        fprintf(stderr, "[extractd_or_fail]\t couldn't parse string %s\n", str);
        exit(1);
    }
    std::time_t ep = std::mktime(&t);
    // std::time_t curtime = time(NULL);
    // long long nsecs = std::difftime(curtime, ep);
    long int nsecs1 = static_cast<int>(ep);

    // std::cout << t.tm_year << ' ' << t.tm_mon << ' ' << t.tm_mday << ' ' << ep << ' ' << nsecs << ' ' << nsecs1 << '\n';
    return nsecs1;
}


double extractd_or_fail(const char *str) {
    char *c;
    double result = strtod(str, &c);
    if (c == str) {
        fprintf(stderr, "[extractd_or_fail]\t couldn't parse string %s\n", str);
        exit(1);
    }

    return result;
}

// int main(int argc, char **argv) {

//   FILE *tbl;
//   int count;
  
//   long lines = N;

//   fprintf(stderr, "Loading data from tpch-dbgen/lineitem.tbl...");
//   fflush(stderr);
//   double l_qua [100][3];
//   long int l_shpdate[100];

//   count = loadData_q6(l_qua, l_shpdate);

// //   std::cout<<count;
//   return 1;
// }

