// 11020128莊松翰
#include <vector> // vector, push_back
#include <stdio.h>
#include <iostream> // cout, endl
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <string.h>
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;


typedef struct hT
  {
    int rid;
    int value ;
    string depart ;
    string studyTime ;
    string grade ;
  }  heapType ;

bool readF(heapType *minSet, string filename) ;
int rawNum(string & filename) ;
int findLeftMost( int num ) ;

class minHeap{

heapType item2 ;

public:

    void equalVal(minHeap &item, heapType &newItem ) {
      item.item2.rid = newItem.rid;
      item.item2.value = newItem.value;
    }
    heapType returnVal() {
      return item2 ;
    }

    void swapVal(heapType tmp, minHeap * item, int newdata, int grandparent ) {
        tmp.rid = item[grandparent].item2.rid ;
        tmp.value = item[grandparent].item2.value ;
        item[grandparent].item2.rid = item[newdata].item2.rid;
        item[grandparent].item2.value = item[newdata].item2.value;
        item[newdata].item2.rid = tmp.rid ;
        item[newdata].item2.value = tmp.value ;
    }

    void heapInsert( minHeap * &item, heapType newItem, int size  ) {
      equalVal( item[size], newItem ) ;

      int newdata = size ;
      int parent = (newdata-1)/2 ;
      while(( item[newdata].item2.value < item[parent].item2.value )) { //看子節點與ancestors需不需要交換
        heapType tmp;
        swapVal(tmp, item, newdata, parent) ;
        newdata = parent;
        parent = (newdata-1)/2;
      } // while

    }
};

class minmaxHeap{

heapType item2 ;

public:

    void equalVal(minmaxHeap &item, heapType &newItem ) {
      item.item2.rid = newItem.rid;
      item.item2.value = newItem.value;
      item.item2.depart = newItem.depart;
      item.item2.studyTime = newItem.studyTime;
      item.item2.grade = newItem.grade;
    }

    void equalVal(minmaxHeap &item, minmaxHeap &item2 ) {
      item.item2.rid = item2.item2.rid;
      item.item2.value = item2.item2.value;
      item.item2.depart = item2.item2.depart;
      item.item2.studyTime = item2.item2.studyTime;
      item.item2.grade = item2.item2.grade;
    }


    heapType returnVal() {
      return item2;
    }
    void swapVal(heapType tmp, minmaxHeap * item, int newdata, int grandparent ) {
        tmp.rid = item[grandparent].item2.rid ;
        tmp.value = item[grandparent].item2.value ;
        tmp.depart = item[grandparent].item2.depart ;
        tmp.studyTime = item[grandparent].item2.studyTime ;
        tmp.grade = item[grandparent].item2.grade ;
        item[grandparent].item2.rid = item[newdata].item2.rid;
        item[grandparent].item2.value = item[newdata].item2.value;
        item[grandparent].item2.depart = item[newdata].item2.depart ;
        item[grandparent].item2.studyTime = item[newdata].item2.studyTime ;
        item[grandparent].item2.grade = item[newdata].item2.grade ;
        item[newdata].item2.rid = tmp.rid ;
        item[newdata].item2.value = tmp.value ;
        item[newdata].item2.depart = tmp.depart ;
        item[newdata].item2.studyTime = tmp.studyTime ;
        item[newdata].item2.grade = tmp.grade ;
    }



    void compareLoop( minmaxHeap * item, int newdata ) {
      int level= ((int) floor(log2(newdata+1)))+1 ; // 計算資料為第幾層
      if( level <= 2 ) return ;
      int grandparent = (newdata - 3) / 4;
      while( level > 2 ) {
        if (level%2 == 0 ) { // 雙數層，祖父要大於子
          if (item[newdata].item2.value >= item[grandparent].item2.value ) { //祖父小於子，交換資料
            heapType tmp;
            swapVal( tmp, item, newdata, grandparent) ;
            newdata = grandparent;
          } // if

          else return ;
        } // if

        else { // 單數層，祖父要小於子
          if (item[newdata].item2.value <= item[grandparent].item2.value ) { //祖父大於子，交換資料
            heapType tmp;
            swapVal( tmp, item, newdata, grandparent) ;
            newdata = grandparent;
          } // if

          else return ;
        } // else

        level= ((int) floor(log2(newdata+1)))+1 ;
      } // while

      return ;
    }


    void heapInsert( minmaxHeap * &item, heapType newItem, int size  ) {
      equalVal( item[size], newItem ) ;
      int newdata = size ;
      int parent = (newdata-1)/2 ;
      int level= ((int) floor(log2(newdata+1)) )+1 ; // 計算資料為第幾層，%2 ??????

      if (level%2 ==0 ) { // 雙數層，父要小於子
          if (parent >= 0 && item[newdata].item2.value <= item[parent].item2.value ) { //父大於子，交換資料
            heapType tmp;
            swapVal( tmp, item, newdata, parent) ;
            newdata = parent;
          } // if
          compareLoop(item, newdata) ; // 比較同樣是雙數層的資料
      } // if

      else { // 單數層，父要大於子
          if (parent >= 0 && item[newdata].item2.value >= item[parent].item2.value ) { //父小於子，交換資料
            heapType tmp;
            swapVal( tmp, item, newdata, parent) ;
            newdata = parent;
          } // if
          compareLoop(item, newdata) ; // 比較同樣是單數層的資料
      } // else
    }
    int compareData(minmaxHeap * &item) {
        if( item[1].item2.value >= item[2].item2.value ) return 2 ;
        else return 1 ;
    }
    int compareData(minmaxHeap * &item, int tmp1, int tmp2, int tmp3, int tmp4 ) {
        if( item[tmp1].item2.value <= item[tmp2].item2.value && item[tmp1].item2.value <= item[tmp3].item2.value && item[tmp1].item2.value <= item[tmp4].item2.value )  return tmp1 ;
        if( item[tmp2].item2.value <= item[tmp1].item2.value && item[tmp2].item2.value <= item[tmp3].item2.value && item[tmp2].item2.value <= item[tmp4].item2.value )  return tmp2 ;
        if( item[tmp3].item2.value <= item[tmp1].item2.value && item[tmp3].item2.value <= item[tmp2].item2.value && item[tmp3].item2.value <= item[tmp4].item2.value )  return tmp3 ;
        if( item[tmp4].item2.value <= item[tmp1].item2.value && item[tmp4].item2.value <= item[tmp2].item2.value && item[tmp4].item2.value <= item[tmp3].item2.value )  return tmp4 ;
    }
    void compareLoop(minmaxHeap * item, int newdata, int size) {

      int level = (int) floor(log2(size))+1 ;
      if ( level == 2 ) return ;
      else {
        int i = 0 ;
        int tmp = level ;
        if( tmp % 2 == 0 ) tmp-- ;
        while( i < tmp / 2 ) {
          int tmp1 = newdata*4+3 ;
          if( tmp1 > size ) tmp1 = 1 ;
          int tmp2 = newdata*4+4 ;
          if( tmp2 > size ) tmp2 = 1 ;
          int tmp3 = newdata*4+5 ;
          if( tmp3 > size ) tmp3 = 1 ;
          int tmp4 = newdata*4+6 ;
          if( tmp4 > size ) tmp4 = 1 ;
          int qus = compareData(item, tmp1, tmp2, tmp3, tmp4) ;
          if(tmp1 <= size && item[newdata].item2.value >= item[qus].item2.value) {
              heapType tmp;
              swapVal( tmp, item, newdata, qus);
              newdata = qus ;
              int level2 = (int) floor(log2(newdata+1))+1 ;
              if(level2 == level - 1 ) {
                if( newdata*2 + 1 <= size && item[newdata].item2.value >= item[newdata*2+1].item2.value) {
                  if(newdata*2 + 2 <= size &&item[newdata*2+1].item2.value >= item[newdata*2+2].item2.value){
                  int tmp2 = newdata*2 + 2;
                  heapType tmp;
                  swapVal( tmp, item, newdata , tmp2);
                  }
                  else {
                    int tmp2 = newdata*2 + 1;
                    heapType tmp;
                    swapVal( tmp, item, newdata , tmp2);
                  }
                }
                }
              }

          else {
            break ;

          }
          i++ ;

        }
      }


    }
    void heapDelete(minmaxHeap * &item, int &size) {
        size-- ;
        equalVal(item[0], item[size]) ;
        if(size == 2 ){
            if( item[0].item2.value > item[1].item2.value ){
                heapType tmp;
                swapVal( tmp, item, 0, 1);
            }
        }
        else {
          int test = compareData(item) ;
          if( item[0].item2.value >= item[test].item2.value ){
            heapType tmp;
            swapVal( tmp, item, 0, test);
          }
          compareLoop(item , 0, size) ;
        }

    }




};
template <typename T>
void outputAns( T heap, int num, int leftMost ) ;
int main(){
    int num = 0;
    int command = 0;
    int leftMost = 0;
    int test = 0 ;
    bool callFirst = false;
    heapType * minSet = NULL;
    string filename ;
    minHeap * heap  ;
    minmaxHeap * heap2 = NULL;
do{
    cout << endl << "**** Heap Construction ****************";
    cout << endl << "* 0. QUIT                             *";
    cout << endl << "* 1. Build a min heap                 *";
    cout << endl << "* 2. Build a min-max heap             *";
    cout << endl << "* 3: Top-K minimums from min-max heap *";
    cout << endl << "***************************************";
    cout << endl << "Input a choice(0, 1, 2, 3): ";
    cin >> command; // get a command

switch (command){
    case 0:
        break;
    case 1:
        num =rawNum(filename) ; // 找出資料總數用於動態陣列
        if(num == -1) break ;
        minSet = new heapType[num] ;
        if(!readF(minSet, filename))  break; //將文字檔裡資料存進minSet
        heap = new minHeap[num] ;
        for( int i= 0; i < num; i++ ) { //將minSet中資料一個一個存進heap中
            heap[i].heapInsert( heap, minSet[i], i );
        }

        leftMost = findLeftMost(num) ; // 找最左下的節點
        cout << "<min heap>"<< endl ;
        outputAns( heap, num, leftMost) ;
        break ;
    case 2:
        num =rawNum(filename) ; // 找出資料總數用於動態陣列
        if(num == -1) break ;
        minSet = new heapType[num] ;
        if(!readF(minSet, filename))  break; //將文字檔裡資料存進minSet
        heap2 = new minmaxHeap[num] ;
        for( int i= 0; i < num; i++ ) { //將minSet中資料一個一個存進heap中
            heap2[i].heapInsert( heap2, minSet[i], i );

        }
        leftMost = findLeftMost(num) ; // 找最左下的節點
        cout << "<min-max heap>"<< endl ;
        outputAns( heap2, num, leftMost) ;
        break ;
    case 3 :
        if( heap2 == NULL || num == 0) break ;
        cout << "Enter the value of K in [1, " << num << "]:"  ;
        cin >> test ;
        if( test < 1 ) break ;


       for ( int j = 0; j < test ; j++ ){
         cout << "Top   "<< j+1 << ": [" << heap2[0].returnVal().rid << "]" << heap2[0].returnVal().depart  << " ,"  << heap2[0].returnVal().studyTime << " ," << heap2[0].returnVal().grade  << " ," << heap2[0].returnVal().value  << endl ;
         heap2[num].heapDelete(heap2, num) ;

       }







        break ;

    default: cout << endl << "Command does not exist!" << endl;
} // end switch
} while (command != 0); // '0': stop the program

} // main()

int rawNum(string & filename ) { // 確認檔案行數
    string fileNO ;
    string line ;
    int num= 0 ;
    int title = 1 ;
    cout << "Input a file number [0: quit]: " ;
    cin >> fileNO;
    if(fileNO == "0") return -1;
    ifstream in;
    filename = "input"+fileNO +".txt";
    in.open(filename.c_str());
    if(in.fail()){
        cout << "### " +filename+" doesn't exist! ###"<< endl;
        return -1;
    } // if

    while( getline( in, line )  ) {
    if( title>3 ) 	num++;
    else title++ ;
	} //while
	in.close();
	return num;
}
bool readF(heapType *minSet, string filename){
    int next = 0,current,Count = 0;
    int key = 1 ;
    int i = 0 ;
    ifstream in;
    string str,rawR, tmp, tmp2;
    in.open(filename.c_str());
    if(in.fail()){
        cout << "### " +filename+" doesn't exist! ###"<< endl;
        return false;
    } // if

    getline(in,str); // title
    getline(in,str); // title
    getline(in,str); // title
    while(!in.eof()){
        getline(in,rawR); // 一次存一行
        if(rawR.size() == 0 ) break;
        current = 0; //最初由 0 的位置開始找
        next = 0, Count = 0;
        while(next != string::npos){ // npos表示string的结束位子
            next = rawR.find_first_of("\t", current);
            str = rawR.substr(current,next - current);
            if(Count == 1) tmp = str ;
            if(Count == 3) {
                tmp2 = str ;
                tmp = tmp + tmp2 ;
                minSet[i].depart = tmp ;
            }
            if(Count == 4) minSet[i].studyTime = str ; // number
            if(Count == 5) {
                minSet[i].grade = str; // number
            } // if
            if(Count == 8) {
                if ( str.size() == 7 ) {
                   string a = str.substr(1,1);
                   string b = str.substr(3,3)  ;
                   str = a + b ;
                   minSet[i].value = stoi(str); // number

                }
                else minSet[i].value = stoi(str); // number
            }
            else if(Count > 8) break;
            current = next + 1; // 從下一個開始找
            Count ++;
        } // while
        minSet[i].rid = key;
        i++;
        key++ ;
    } // while

    return true;
} // read records from a file
int findLeftMost(int num) { // 計算最底層最左邊的資料
    int level= ((int) floor(log2(num))) +1;  //  計算資料為第幾層，%2 ??????
    int leftMost= 1;
    for( int i= 0; i< level-1; i++ ) {
        leftMost*= 2;
    }

    return leftMost ;
}

template <typename T>
void outputAns( T heap, int num, int leftMost ) {
    cout <<  "root: [" << heap[0].returnVal().rid << "] " << heap[0].returnVal().value << endl ;
    cout <<  "bottom: [" << heap[num-1].returnVal().rid << "] " << heap[num-1].returnVal().value << endl ;
    cout <<  "leftmost bottom: [" << heap[leftMost-1].returnVal().rid << "] " << heap[leftMost-1].returnVal().value << endl ;
}
