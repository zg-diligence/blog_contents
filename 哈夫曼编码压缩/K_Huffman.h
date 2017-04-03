/*
 * Copyright(c) 2017 Gang Zhang
 * All rights reserved.
 * Author:Gang Zhang
 * Date:2017.3.26
 */

/*
 * Abstract:
 *   This article implements k-forks huffman coding compression.
 */

#ifndef KHUFFMAN
#define KHUFFMAN

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
template <size_t K> class KHuffman
{
    /* node of the huffman tree. */
    struct Node
    {
        unsigned char ch = '\0';
        size_t weight = 0;
        size_t height = 0;
        vector<Node*> childs;
        Node(unsigned char c = '\0', size_t w = 0, size_t d = 0) 
            :ch(c), weight(w), height(d) {childs.resize(K, nullptr);}
    };

public:
    /* constructor. */
    KHuffman(const string &str):text(str)
    {
        for (unsigned char ch : text)
            hashWeight[ch]++;
        if (hashWeight.size() == 1) //when there is only one line with same chars.
            hashWeight[255] = 0;
    }

    /* encode the text, return encoded text. */
    string encode()
    {
        buildTree(); //print(root);
        getHuffCode(root,"");
        for (unsigned char ch : text)
            encodeStr += hashTable[ch];

        long long oldNum = text.size();
        size_t codeNum = 0, max = 0;
        for (const auto &code : hashTable)
        {
            codeNum++;
            if (code.second.size() > max)
                max = code.second.size();
        }
        long long newNum = (max / 8 + 1)*codeNum + 3;
        size_t tmp = static_cast<size_t>(log(K) / log(2));
        tmp = log(K) / log(2) > tmp ? tmp + 1 : tmp;
        newNum += encodeStr.size() * (tmp) / 8;
        cout << "Ñ¹ËõÂÊ = " << (double)newNum / oldNum << endl;
        return encodeStr;
    }

    /* decode the compressed text, return decoded text. */
    string decode()
    {
        Node *cur = root;
        for (unsigned char ch : encodeStr)
        {
            size_t n = ch - 48;
            if (cur->childs[n] != nullptr)
                cur = cur->childs[n];
            else
            {
                decodeStr += cur->ch;
                cur = root->childs[n];
            }
        }
        decodeStr += cur->ch;
        return decodeStr;
    }

private:
    map<unsigned char, size_t> hashWeight; //save weight of each char.
    map<unsigned char, string> hashTable;  //save huffman code of each char.
    Node *root = nullptr; //root node of the k-huffman tree.
    string text;          //source file text.
    string encodeStr;     //compressed text.
    string decodeStr;     //decompressed text.

    /* comparison operator <. */
    bool less(Node *left, Node *right)
    {
        if (left->weight == right->weight)
            return left->height < right->height;
        return left->weight < right->weight;
    }

    /* comparison operator <=. */
    bool lessEqual(Node *left, Node *right)
    {
        if (left->weight == right->weight)
            return left->height <= right->height;
        return left->weight < right->weight;
    }

    /* pushDown an element in a min-heap. */
    void pushDown(vector<Node*> &heap, size_t first, size_t last)
    {
        size_t cur = first;
        Node *item = heap[first];
        while (cur <= last / 2)
        {
            size_t next = cur * 2;
            if (next < last && less(heap[next + 1], heap[next]))
                next++;
            if (lessEqual(item, heap[next]))
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
            if (!less(item, heap[next]))
                break;
            heap[cur] = heap[next];
            cur = next;
        }
        heap[cur] = item;
    }

    /* build K forks huffman tree. */
    void buildTree()
    {
        /* build min-heap. */
        vector<Node*> heap{ new Node };
        for (const auto &elem : hashWeight)
            heap.push_back(new Node(elem.first, elem.second));
        size_t size = heap.size() - 1;
        for (size_t pos = size / 2; pos >= 1; --pos)
            pushDown(heap, pos, size);

        /* make sure the last time has K nodes. */
        if ((size - 1) % (K - 1) != 0)
        {
            size_t n = (size - 1) % (K - 1) + 1, i = 0;
            Node *t = new Node;
            while (n--)
            {
                t->childs[i++] = heap[1];
                t->weight += heap[1]->weight;
                heap[1] = heap[size--];
                pushDown(heap, 1, size);
            }
            t->height = 1;
            heap[++size] = t;
            pushUp(heap, size);
        }

        /* build k-huffman tree. */
        while (size > 1)
        {
            size_t n = K, i = 0;
            Node *t = new Node;
            while (n--)
            {
                t->childs[i++] = heap[1];
                t->weight += heap[1]->weight;
                heap[1] = heap[size--];
                pushDown(heap, 1, size);
            }
            size_t height = 0;
            for (Node *item : t->childs)
                if (item->height > height)
                    height = item->height;
            t->height = height + 1;
            heap[++size] = t;
            pushUp(heap, size);
        }
        root = heap[1];
    }

    /* get huffman code of each char.*/
    void getHuffCode(Node *t, string code)
    {
        if (t == nullptr)
            return;
        bool flag = false;
        for (size_t i = 0; i != K; ++i)
            if (t->childs[i] != nullptr)
            {
                getHuffCode(t->childs[i], code + char(i + 48));
                flag = true;
            }
        if (flag == false)
            hashTable[t->ch] = code;
    }

    /* debug: print k-huffman tree. */
    void print(Node *t)
    {
        if (t != nullptr)
        {
            cout << t->ch << " ";
            for (auto item : t->childs)
                print(item);
        }
    }
};

#endif // !K-HUFFMAN