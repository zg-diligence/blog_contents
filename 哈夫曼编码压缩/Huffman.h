/*
 * Copyright(c) 2017 Gang Zhang
 * All rights reserved.
 * Author:Gang Zhang
 * Date:2017.3.21 - 3.23
 */

/*
 * Abstract:
 *   This article implements static huffman coding compression.
 */

#ifndef HUFFMAN
#define HUFFMAN

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Huffman
{
    /* node of huffman tree. */
    struct Node
    {
        unsigned char ch;
        size_t weight;
        Node *left;
        Node *right;
        Node(unsigned char c = '\0', size_t w = 0, Node *lc = nullptr, Node *rc = nullptr) 
            :ch(c), weight(w), left(lc), right(rc) {}
    };

public:
    /* constructor */
    Huffman(const string &file) :filePath(file) {}

    /* destructor. */
    ~Huffman() { clear(root); }

    /* return compression ratio. */
    double compressionRatio()const { return ratio; }

    /* compress the pointed file. */
    bool zip(const string &zipName)
    {
        ifstream in(filePath, ios::binary);
        if (!in)  //the file path is invalid.
            return false;

        /* read file line by line and count how many times each char occurs. */
        string line;
        while (getline(in, line))
        {
            text += line + '\n';
            for (unsigned char ch : line + '\n')
                hashWeight[ch]++;
        }
        if (!line.empty())  //if the last line is not empty line.
        {
            text.pop_back();
            hashWeight['\n']--;
        }
        oldNum = text.size();

        /* build huffman tree and get the hash table. */
        buildHufTree1();
        getHashTable(root);

        /* encode and write it into the compressed file. */
        encodeAndWrite(zipName);
        in.close();
        return true;
    }

    /* decompress the pointed file. */
    bool unzip(const string &unzipName)
    {
        ifstream in(filePath, ios::binary);
        if (!in)  //the file path is invalid.
            return false;

        /* get the obligated information. */
        size_t appendedBits = in.get();
        size_t codeNum = in.get();
        size_t codeLenth = in.get();
        if (codeNum == 0)  //if 256 chars are here, unsigned char is out of bounds.
            codeNum = 256;

        /* read huffman code and build huffman tree. */
        while (codeNum--)
        {
            string code;
            unsigned char ch = in.get();
            for (size_t i = 0; i != codeLenth; ++i)
                code += char2binary(in.get());
            size_t pos = code.size();
            while (code[--pos] != '1')
                code.pop_back();
            code.pop_back();
            hashTable[ch] = code;
        }
        buildHuffTree2();

        /* read compressed file. */
        string line, buf;
        while (getline(in, line)) //getline read '\n' but and abandon it.
            for (unsigned char ch : line + '\n')
                buf += char2binary(ch);
        appendedBits += 7;  //cannot add 8.
        buf.erase(buf.size() - appendedBits, appendedBits);

        /* decode the text and write it into the decompressed file. */
        decodeAndWrite(buf, unzipName);
        in.close();
        return true;
    }

private:
    double ratio = 0.0;           //compression ratio.
    long long oldNum = 0;         //number of chars the old file has.
    long long newNum = 0;         //number of chars the compressed file has.
    map<unsigned char, size_t> hashWeight; //occuring times of each char.
    map<unsigned char, string> hashTable;  //hash code of each char.
    Node *root = nullptr;         //root node of the hashTree and the forward_list.
    string text;                  //contents of the file to compress.
    string filePath;              //name of the file to compress or decompress.
    string codeStack;             //auxiliary stack to get hash code of each char.
    
    /* transform char into binary string. */
    string char2binary(unsigned char c)
    {
        string result(8, '0');
        size_t pos = 7; //inverse conversion.
        while (c != 0)
        {
            result[pos--] = c % 2 + 48;
            c /= 2;
        }
        return result;
    }

    /* transform binary string into char. */
    unsigned char binary2char(string s)
    {
        int num = stoi(s);
        size_t term = 1, result = 0;
        while (num != 0)
        {
            result += num % 10 * term;
            term *= 2;
            num /= 10;
        }
        return (unsigned char)result;
    }

    /* pushDown an element in a min-heap. */
    void pushDown(vector<Node*> &heap, size_t first, size_t last)
    {
        size_t cur = first;
        Node *item = heap[first];
        while (cur <= last / 2)
        {
            size_t next = cur * 2;
            if (next < last && heap[next + 1]->weight < heap[next]->weight)
                next++;
            if (item->weight <= heap[next]->weight)
                break;
            heap[cur] = heap[next];
            cur = next;
        }
        heap[cur] = item;
    }

    /* pushUp an element in a min-heap. */
    void pushUp(vector<Node*> &heap, size_t pos)
    {
        size_t cur = pos;
        Node *item = heap[pos];
        while (cur > 1)
        {
            size_t next = cur / 2;
            if (item->weight >= heap[next]->weight)
                break;
            heap[cur] = heap[next];
            cur = next;
        }
        heap[cur] = item;
    }

    /* build hufman tree. */
    void buildHufTree1()
    {
        /* build min-heap. */
        vector<Node*> heap{ new Node };
        for (const auto &elem : hashWeight)
            heap.push_back(new Node(elem.first, elem.second));
        size_t size = heap.size() - 1;
        for (size_t pos = size / 2; pos >= 1; --pos)
            pushDown(heap, pos, size);

        /* build huffman tree. */
        while (size > 1)
        {
            Node *first = heap[1];
            heap[1] = heap[size--];
            pushDown(heap, 1, size);
            Node *second = heap[1];
            heap[1] = heap[size--];
            pushDown(heap, 1, size);

            size_t newWeight = first->weight + second->weight;
            heap[++size] = new Node('\0', newWeight, first, second);
            pushUp(heap, size);
        }
        root = heap[1];
    }

    /* build huffman tree using pointed hash table. */
    void buildHuffTree2()
    {
        root = new Node('\0', 0);
        for (const auto &elem : hashTable)
        {
            Node *cur = root;
            for (unsigned char c : elem.second)
            {
                if (c == '0')
                {
                    if (cur->left == nullptr)
                        cur->left = new Node('\0', 0);
                    cur = cur->left;
                }
                else
                {
                    if (cur->right == nullptr)
                        cur->right = new Node('\0', 0);
                    cur = cur->right;
                }
            }
            cur->ch = elem.first;
            cur->weight = 1;  //mark end of the code on the huffman tree.
        }
    }

    /* get the hash table. */
    void getHashTable(Node *t)
    {
        if (t->left == nullptr && t->right == nullptr)
        {
            hashTable[t->ch] = codeStack;
            codeStack.pop_back();
            return;
        }

        codeStack.push_back('0');
        getHashTable(t->left);
        codeStack.push_back('1');
        getHashTable(t->right);

        if (!codeStack.empty())
            codeStack.pop_back();
    }

    /* encode and write text into the compressed file. */
    void encodeAndWrite(const string &zipName)
    {
        while (filePath.back() != '\\')
            filePath.pop_back();
        filePath += zipName + ".zg";
        ofstream out(filePath, ios::binary);
        out << "567";  //obligate three bytes for leaving critical information.

        /* count max length of the hash code string. */
        size_t len = 0, codeNum = 0;
        for (const auto &elem : hashTable)
        {
            codeNum++;
            if (elem.second.size() > len)
                len = elem.second.size();
        }
        size_t codeLenth = len / 8 + 1;  //bytes for each hash code.
        newNum += codeLenth*codeNum + 3;
        /* write the hash table into the compressed file. */
        for (const auto elem : hashTable)
        {
            out << elem.first;
            string str = elem.second + '1';  //add 100... to mark end of the code.
            str.resize(8 * codeLenth, '0');
            for (size_t i = 0; i != codeLenth; ++i)
                out << binary2char(str.substr(i * 8, 8));
        }

        /* encode source file and write it into the compressed file. */
        string buf;
        for (unsigned char ch : text)
        {
            buf += hashTable[ch];
            while(buf.size() >= 8)
            {
                out << binary2char(buf.substr(0, 8));
                buf.erase(0, 8);
                newNum++;
            }
        }

        /* check if it neccessary to append bits in the end. */
        size_t appendBits = 0;
        if (!buf.empty())
        {
            appendBits = 8 - buf.size();
            buf.resize(8, '0');
            out << binary2char(buf);
            newNum++;
        }
        ratio = newNum / (double)oldNum;

        /* go back to head of the file and write obligated information. */
        out.seekp(ios::beg);
        out << (unsigned char)appendBits << (unsigned char)codeNum << (unsigned char)codeLenth;
        out.close();
    }

    /* decode and write text into the decompressed file. */
    void decodeAndWrite(const string &buf, const string &unzipName)
    {
        while (filePath.back() != '\\')
            filePath.pop_back();
        filePath += unzipName;
        ofstream out(filePath, ios::binary);

        Node *cur = root;
        for (size_t i = 0; i != buf.size(); ++i)
        {
            if (cur->weight != 0)
            {
                out << cur->ch;
                cur = root;
            }
            cur = (buf[i] - '0') ? cur->right : cur->left;
        }
        out.close();
    }

    /* release the memory. */
    void clear(Node *t)
    {
        if (t != nullptr)
        {
            clear(t->left);
            clear(t->right);
            delete t;
        }
    }
};

#endif // ! HUFFMAN