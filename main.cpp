/*
Author : 張承勛 B123040053
Date : Nov. 28, 2023
Purpose : 利用Huffman演算法壓縮&解壓縮檔案
*/
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<functional>
using namespace std;

vector<char> Data;//輸入資料
vector<int> cnt;//紀錄字元出現頻率
vector<string> table;//編碼表
int num_of_kind, extra_bit;//字元種類數量, 編碼最後多出多少bit

struct Node{//huffman tree節點
    int key;
    string name;
    Node *left, *right;

    Node(int k = 0, string n = "", Node* l = nullptr, Node* r = nullptr): key(k), name(n), left(l), right(r){};

    char mnc(){//尋找name中最小字元
        char c = name[0];
        for(int i = 1; i < name.size(); ++i){
            c = min(c, name[i]);
        }
        return c;
    }

    bool operator<(Node x){
        if(key != x.key) return key < x.key;
        return mnc() < x.mnc();
    }

    void operator=(Node x){
        key = x.key;
        name = x.name;
        left = x.left;
        right = x.right;
    }
};

struct Huffman_Tree{
    Node* root;

    Huffman_Tree(Node* r = nullptr): root(r){};
    Huffman_Tree(int k, string n){
        root = new Node(k, n);
    };

    bool operator<(Huffman_Tree x){
        return *root < *x.root;
    }

    void Clear_Tree(Node* ptr){
        if(ptr == nullptr) return;
        Clear_Tree(ptr->left);
        Clear_Tree(ptr->right);
        delete ptr;
    }
}final_tree;

template<class T>
class Min_Heap{
    public:
        Min_Heap(int mxsize, vector<T>& a){
            heap_size = a.size();
            heap = new T[mxsize];
            for(int i = 0; i < heap_size; ++i) heap[i] = a[i];
            for(int i = heap_size / 2 - 1; ~i; --i) heapify(i);//build Min-Heap
        }

        ~Min_Heap(){
            delete [] heap;
        }

		int Size(){
			return heap_size;
		}

        void Insert(T y){//insert y to Min-Heap
            heap[heap_size++] = y;
            int x = heap_size - 1;
            while(x > 0 && heap[x] < heap[(x - 1) >> 1]){
                swap(heap[x], heap[(x - 1) >> 1]);
                x = (x - 1) >> 1;
            }
        }

        T Top(){//get minimum
            return heap[0];
        }

        void Pop(){//delete minimum from Min-Heap
            swap(heap[0], heap[--heap_size]);
            heapify(0);
        }

    private:
        T* heap;
        int heap_size;

        void heapify(int x){
            while(1){
                int mnid = x;
                if(((x << 1) | 1) < heap_size && heap[(x << 1) | 1] < heap[mnid]) mnid = ((x << 1) | 1);
                if(((x << 1) + 2) < heap_size && heap[(x << 1) + 2] < heap[mnid]) mnid = ((x << 1) + 2);

                if(mnid != x){
                    swap(heap[mnid], heap[x]);
                    x = mnid;
                }else{
                    break;
                }
            }
        }
};

Huffman_Tree Merge_Tree(Huffman_Tree a, Huffman_Tree b){//合併tree
    if(b.root->mnc() < a.root->mnc()) swap(a, b);
    Huffman_Tree ret(a.root->key + b.root->key, a.root->name + b.root->name);
    ret.root->left = a.root;
    ret.root->right = b.root;
    return ret;
}

void count_freq(){//計算字元出現頻率
    cnt.assign(256, 0);
    for(auto x: Data){
        int tmp = x;
        if(tmp < 0) tmp += 256;
        cnt[tmp]++;
    }
}

void Read_Data_c(string file){//以二進位讀取要壓縮的檔案
    ifstream inpt(file, ios::in | ios::binary);
    char c;
    inpt.get(c);
    while(!inpt.eof()){
        Data.push_back(c);
        inpt.get(c);
    }

    inpt.close();
}

Huffman_Tree Build_Huffman_Tree(){//建造huffman tree
    vector<Huffman_Tree> build_heap_arr;

    num_of_kind = 0;
    for(int i = 0; i < 256; ++i){
        if(cnt[i]){
            string s("");
            s.push_back((char)(i));
            Huffman_Tree tmp(cnt[i], s);
            build_heap_arr.push_back(tmp);
            ++num_of_kind;
        }
    }

    Min_Heap<Huffman_Tree> min_heap(256, build_heap_arr);

    while(min_heap.Size() > 1){//每次合併key最小的兩個節點
        Huffman_Tree mg1 = min_heap.Top();
        min_heap.Pop();
        Huffman_Tree mg2 = min_heap.Top();
        min_heap.Pop();
        mg1 = Merge_Tree(mg1, mg2);
        min_heap.Insert(mg1);
    }

    return min_heap.Top();
}

void encode_dfs(Node* ptr, string code){//透過huffman tree替字元編碼
    if(ptr == nullptr) return;
    if(ptr->name.size() == 1){//leaf node
        int tmp = ptr->name[0];
        if(tmp < 0) tmp += 256;

        if(code.size() == 0){//檔案只有一種字元的特殊情況
            table[tmp] = "0";
        }else{
            table[tmp] = code;
        }
    }
    encode_dfs(ptr->left, code + "0");//往左子樹
    encode_dfs(ptr->right, code + "1");//往右子樹
}

int int_byte(int num){//計算整數存在壓縮檔案中佔多少byte
    int ret = 0;
    while(num){
        num /= 10;
        ++ret;
    }
    return ret;
}

void Output_Compressed_Data(string file){//輸出壓縮後的檔案
    vector<char> compressed_data;
    int c = 0;
    int bitnum = 0, compdt_size = 0;
    compdt_size += int_byte(num_of_kind) + 1;

    for(char x: Data){//將輸入資料透過編碼表壓縮
        int tmp = x;
        if(tmp < 0) tmp += 256;
        for(char y: table[tmp]){
            if(y == '0'){
            }else{
                c |= (1 << (7 - bitnum));
            }
            ++bitnum;
            if(bitnum == 8){
                compressed_data.push_back((char)c);
                bitnum = 0;
                c = 0;
                compdt_size++;
            }
        }
    }

    extra_bit = 0;
    if(bitnum){
        extra_bit = 8 - bitnum;
        compressed_data.push_back((char)c);
        compdt_size++;
    }

    ofstream oupt(file, ios::out | ios::binary);

    oupt << num_of_kind << ' ' << extra_bit << endl;

    for(int i = 0; i < 256; ++i){//輸出編碼表到檔案並計算壓縮檔案大小
        if(table[i] != ""){
            oupt << i << ' ' << table[i] << endl;
            compdt_size += int_byte(i) + 2 + table[i].size();
        }
    }

    cout << "Original data size: " << Data.size() <<" bytes\n";
    cout << "Compressed data size: " << compdt_size <<" bytes\n";
    cout << "Compression ratio: " << 100.0 * compdt_size / Data.size() << "%" << endl;

    for(int i = 0; i < 256; ++i){//輸出編碼表到標準輸出
        if(table[i] != ""){
            cout << (char)i << ' ' << table[i] << endl;
        }
    }

    for(auto x: compressed_data) oupt << x;//輸出壓縮後的檔案

    oupt.close();
}

void Read_Data_u(string file){//讀取要解壓縮的檔案
    ifstream inpt(file, ios::in | ios::binary);
    inpt >> num_of_kind >> extra_bit;
    final_tree.root = new Node;
    
    for(int i = 0; i < num_of_kind; ++i){//將huffman tree建立回來
        int c;
        string code;
        inpt >> c >> code;
        Node* ptr = final_tree.root;
        for(char x: code){
            if(x == '0'){
                if(ptr->left == nullptr) ptr->left = new Node;//若沒有節點則建立一個
                ptr = ptr->left;
            }else{
                if(ptr->right == nullptr) ptr->right = new Node;
                ptr = ptr->right;
            }
        }
        ptr->name.push_back((char)(c));
    }

    char c;
    inpt.get(c);//讀取換行字符
    inpt.get(c);
    while(!inpt.eof()){
        Data.push_back(c);
        inpt.get(c);
    }

    inpt.close();
};

void Decode_And_Output(string file){//解碼並輸出檔案
    ofstream oupt(file, ios::out | ios::binary);
    Node* ptr = final_tree.root;

    for(int i = 0, sz = Data.size(); i < sz; ++i){//將壓縮過個檔案轉換為原來的檔案
        for(int j = 7; ~j; --j){
            if(i == sz - 1 && j < extra_bit) break;//只剩下多餘的bit時跳離迴圈

            if((Data[i] >> j) & 1) ptr = ptr->right;//1 往右
            else ptr = ptr->left;//0 往左

            if(ptr->name.size() == 1){
                oupt << ptr->name[0];
                ptr = final_tree.root;//跳回根節點繼續
            }
        }
    }
    
    oupt.close();
}

void Compression(string inputfile, string outputfile){//壓縮
    Read_Data_c(inputfile);
    count_freq();
    final_tree = Build_Huffman_Tree();
    table.assign(256, "");
    encode_dfs(final_tree.root, "");
    Output_Compressed_Data(outputfile);
}

void Decompression(string inputfile, string outputfile){//解壓縮
    Read_Data_u(inputfile);
    Decode_And_Output(outputfile);
}

int main(int argc, char *argv[]){
    if(argc != 6){//輸入格式錯誤
        cout << "ERROR!!";
        return 0;
    }

    if((string)argv[1] == "-c") Compression(argv[3], argv[5]);
    else if((string)argv[1] == "-u") Decompression(argv[3], argv[5]);
    else{//輸入格式錯誤
        cout << "ERROR!!\n";
        return 0;
    }
    final_tree.Clear_Tree(final_tree.root);//清空tree
    return 0;
}