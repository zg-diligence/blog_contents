/*
 * Copyright(c) 2017 Gang Zhang
 * All rights reserved.
 * Author:Gang Zhang
 * Date:2017.3.25
 */

/*
 * Abstract:
 *   This article implements dynamic huffman coding compression.
 */

#ifndef DYHUFFMAN
#define DYHUFFMAN

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <map>
using namespace std;
#define MAX_SIZE 1024

class DyHuffman
{
    /* node of the huffman tree. */
    struct Node
    {
        /* must assign default initial value. */
        unsigned char ch = '\0';
        size_t weight = 0;
        size_t left = 0;
        size_t right = 0;
        size_t parent = 0;
    };

public:

    /* constructor. */
    DyHuffman(const string &file) :filePath(file)
    { 
        tree[root].ch = NYT; 
        for (size_t i = 0; i != 256; ++i)
            position[unsigned char(i)] = -1;
    }

    /* deconstructor. */
    ~DyHuffman() { delete[]tree; }

    /* return compression ratio. */
    double compressionRatio()const { return ratio; }

    /* compress the file.*/
    bool zip(const string &zipName)
    {
        ifstream in(filePath, ios::binary);
        if (!in)   //path of the file is invalid.
            return false;

        /* read the source file. */
        string line;
        while (getline(in, line))
            text += line + '\n';
        if(!line.empty())  //if the last line is not empty line.
            text.pop_back();
        oldNum = text.size();

        /* encode the text and write it into the compressed file. */
        encodeAndWrite(zipName);
        in.close();
        return true;
    }

    /* decompress the file.*/
    bool unzip(const string &unzipName)
    {
        ifstream in(filePath, ios::binary);
        if (!in)  //path of the file is invalid.
            return false;
        
        /* read compressed file. */
        size_t appendedBits = in.get();
        string buf, line;
        while (getline(in, line))
            for (unsigned char ch : line + '\n')
                buf += char2binary(ch);
        appendedBits += 8;
        buf.erase(buf.size() - appendedBits, appendedBits);
        
        /* decode the text and write it into the decompressed file. */
        decodeAndWrite(buf, unzipName);
        in.close();
        return true;
    }
    
private:
    double ratio = 0.0;              //compression ratio.
    long long oldNum = 0;            //number of chars the old file has.
    long long newNum = 0;            //number of chars the compressed file has.
    Node *tree = new Node[MAX_SIZE]; //Node array for huffman node.
    map<unsigned char, int> position;//position of chars in huffman tree.
    const unsigned char NYT = 255;   //Not yet transmitted, NYT can be any char.
    size_t root = MAX_SIZE - 1;      //init the last node as root node.
    size_t NYTIndex = MAX_SIZE - 1;  //position of NYT node in huffman tree.
    string filePath;
    string text;                     //save content of the source file.

    /* transform char into binary string. */
    string char2binary(unsigned char ch)
    {
        string result(8, '0');
        size_t pos = 7; //inverse conversion.
        while (ch != 0)
        {
            result[pos--] = ch % 2 + 48;
            ch /= 2;
        }
        return result;
    }

    /* transform binary string into char. */
    unsigned char binary2char(string bin)
    {
        /* make sure size of bin is 8. */
        int num = stoi(bin);
        size_t term = 1, result = 0;
        while (num != 0)
        {
            result += num % 10 * term;
            term *= 2;
            num /= 10;
        }
        return (unsigned char)result;
    }

    /* get corresponding huffman code of a char. */
    string char2code(size_t pos)
    {
        /* make sure the char is in the tree. */
        string rcode; //save the huffman code reversely.
        size_t cur = pos;
        while (cur != root)
        {
            if (cur == tree[tree[cur].parent].left)
                rcode += '0';
            else
                rcode += '1';
            cur = tree[cur].parent;
        }
        return string(rcode.rbegin(), rcode.rend());
    }

    /* get corresponding char of a huffman code. */
    unsigned char code2char(string bin)
    {
        /* make sure the binary code is correct. */
        size_t cur = root;
        for (unsigned char ch : bin)
            if (ch == '0')
                cur = tree[cur].left;
            else
                cur = tree[cur].right;
        return tree[cur].ch;
    }

    /* get position of node with the biggest number, if not found, return -1. */
    int highestInBlock(size_t weight)
    {
        for (int i = MAX_SIZE - 1; i != NYTIndex; --i)
            if (tree[i].weight == weight)
                return i;
        return -1;
    }

    /* swap two subtrees without changing number of nodes.*/
    void swap(size_t first, size_t second)
    {
        /* both node cannot be the root. */
        tree[tree[first].left].parent = second;
        tree[tree[first].right].parent = second;
        tree[tree[second].left].parent = first;
        tree[tree[second].right].parent = first;

        size_t tmp = tree[first].left;
        tree[first].left = tree[second].left;
        tree[second].left = tmp;

        tmp = tree[first].right;
        tree[first].right = tree[second].right;
        tree[second].right = tmp;

        /* there is no need to exchange weight. */
        unsigned char ch = tree[first].ch;
        tree[first].ch = tree[second].ch;
        tree[second].ch = ch;

        if(tree[first].left == 0 && tree[first].right == 0)
            position[tree[first].ch] = first;
        if(tree[second].left == 0 && tree[second].right == 0)
            position[tree[second].ch] = second;
    }

    /* insert a new char into the tree, return position of old NYT node. */
    size_t spawn(unsigned char ch)
    {
        size_t oldNYT = NYTIndex;

        tree[oldNYT].ch = '\0';
        tree[oldNYT].weight = 1;
        tree[oldNYT].right = oldNYT - 1;
        tree[oldNYT].left = oldNYT - 2;

        tree[oldNYT - 1].ch = ch;
        tree[oldNYT - 1].weight = 1;
        tree[oldNYT - 1].parent = oldNYT;
        position[ch] = oldNYT - 1;

        tree[oldNYT - 2].ch = NYT; //defaultly, weight of the new NYT is 0.
        tree[oldNYT - 2].parent = oldNYT;
        NYTIndex = oldNYT - 2;
        return oldNYT;
    }

    /* update the huffman tree. */
    void updateTree(unsigned char ch)
    {
        int cur = position[ch];
        if (cur != -1)
        {
            int max = highestInBlock(tree[cur].weight);
            if (cur != max && tree[cur].parent != max)
            {   //cur might be the last char inserted into.
                swap(cur, max);
                cur = max;
            }
            tree[cur].weight++;
        }
        else
            cur = spawn(ch); //insert a new char into the tree.

        /* refresh the huffman tree until root recursively. */
        while (cur != root)  
        {
            cur = tree[cur].parent;
            int max = highestInBlock(tree[cur].weight);
            if (cur != max && tree[cur].parent != max)
            {
                swap(cur, max);
                cur = max;
            }
            tree[cur].weight++;
        }
    }

    /* encode the text and write it into the compressed file. */
    void encodeAndWrite(const string &zipName)
    {
        while (filePath.back() != '\\')
            filePath.pop_back();
        filePath += zipName + ".zg";
        ofstream out(filePath, ios::binary);
        out << '\0';        //obligate one byte for appendedBits.

        /* encode and write the text into compressed file.*/
        string buf;  //buffer of encoded string.
        for (unsigned char ch : text)
        {
            int pos = position[ch];  
            if (pos != -1)
                buf += char2code(pos);
            else
                buf += char2code(NYTIndex) + char2binary(ch);
            updateTree(ch);
            while (buf.size() >= 8)
            {
                out << binary2char(buf.substr(0, 8));
                buf.erase(0, 8);
                newNum++;
            }
        }
        
        /* check if it neccessary to append bits at end of the code. */
        size_t appendedBits = 0;
        if (!buf.empty())
        {
            appendedBits = 8 - buf.size();
            buf.resize(8, '0');
            out << binary2char(buf);
            newNum++;
        }
        ratio = newNum / (double)oldNum;

        /* go back to head of the file, write obligated information. */
        out.seekp(ios::beg);
        out << unsigned char(appendedBits);
        out.close();

    }

    /* decode the text and write it into the decompressed file. */
    void decodeAndWrite(const string &buf, const string &unzipName)
    {
        while (filePath.back() != '\\')
            filePath.pop_back();
        filePath += unzipName;
        ofstream out(filePath, ios::binary);

        /* decode and write the text into the decompressed file. */
        for (size_t i = 0; i < buf.size();)
        {
            unsigned char ch;
            size_t cur = root;
            while (tree[cur].left != 0 || tree[cur].right != 0)
            {
                if (buf[i++] == '0')
                    cur = tree[cur].left;
                else
                    cur = tree[cur].right;
            }
            if (cur == NYTIndex)
            {
                ch = binary2char(buf.substr(i, 8));
                i += 8;
            }
            else
                ch = tree[cur].ch;
            out << ch;
            updateTree(ch);
        }
        out.close();
    }

    /* debug print the huffman tree layer by layer.*/
    void print()
    {
        queue<int> q;
        q.push(root);
        size_t cur = root;
        size_t cnt = 0;

        cout << "\n-------------------------------------" << endl;
        while (!q.empty())
        {
            cnt++;
            cur = q.front(); q.pop();
            if (tree[cur].left != 0)
                q.push(tree[cur].left);
            if (tree[cur].right != 0)
                q.push(tree[cur].right);
            cout << cur << "(" << tree[cur].weight << ")\t" << tree[cur].parent << "\t"
                << tree[cur].left << "\t" << tree[cur].right << endl;
        }
        cout << "***cnt = " << cnt << "***" << endl;
        cout << "\n-------------------------------------" << endl;
    }
};

#endif // !DYHUFFMAN