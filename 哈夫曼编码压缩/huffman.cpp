#include "OpenFile.h"
#include "Huffman.h"
#include "K-Huffman.h"
#include "DyHuffman.h"

int main()
{
    cout << "方式选择:\n  1.普通哈夫曼\n  2.K-叉哈夫曼\n  3.动态哈夫曼\n  4.退出" << endl;
    cout << "请选择:"; int order; cin >> order;
    bool flag = true;
    while (flag)
    {
        switch (order)
        {
            case 1:
            {
                cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                cout << "请选择:"; int choice; cin >> choice;
                bool flag1 = true;
                while (flag1)
                {
                    switch (choice)
                    {
                        case 1:
                        {
                            cout << "\n请选择要压缩的文件:\n";
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "请务必选择文件！" << endl;
                                break;
                            }
                            cout << "请输入压缩后的文件名:";
                            string zipName; cin >> zipName;
                            Huffman ziphuf(path);
                            cout << "压缩中..." << endl;
                            ziphuf.zip(zipName);
                            cout << "压缩成功, 压缩率 = " << 100 * ziphuf.compressionRatio() << "%" << endl;
                            break;
                        }
                        case 2:
                        {
                            cout << "\n请选择要解压的文件:" << endl;
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "请务必选择文件！" << endl;
                                break;
                            }
                            cout << "请输入解压后的文件名(包括后缀):";
                            string unzipName; cin >> unzipName;
                            Huffman unziphuf(path);
                            cout << "解压中..." << endl;
                            unziphuf.unzip(unzipName);
                            cout << "解压成功." << endl;
                            break;
                        }
                        case 3:
                        {
                            flag1 = false;
                            break;
                        }
                        case 4:
                            return 0;
                        default:
                            cout << "输入错误,请重新选择!" << endl;
                    }
                    if (!flag1)break;
                    cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                    cout << "请选择:"; cin >> choice;
                }
                break;
            }
            case 2:
            {
                cout << "\n请选择要压缩的文件:" << endl;
                string path = openFile(L"D://text");
                if (path.empty())
                {
                    cout << "请务必选择文件！" << endl;
                    break;
                }
                ifstream in(path, ios::binary);
                size_t pos = path.size();
                while (path[--pos] != '.') {}
                path.insert(pos - 1, "-dev");
                ofstream out(path, ios::binary);

                string line, text;
                while (getline(in, line))
                    text += line + '\n';
                if (!line.empty())
                    text.pop_back();
 
                KHuffman<32> huf(text);
                cout << "压缩中..." << endl;
                huf.encode();
                cout << "压缩完成." << endl;
                cout << "\n解压中..." << endl;
                out << huf.decode();
                cout << "解压完成." << endl;

                in.close();
                out.close();
                break;
            }
            case 3:
            {
                cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                cout << "请选择:"; int choice; cin >> choice;
                bool flag1 = true;
                while (flag1)
                {
                    switch (choice)
                    {
                        case 1:
                        {
                            cout << "\n请选择要压缩的文件:\n";
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "请务必选择文件！" << endl;
                                break;
                            }
                            cout << "请输入压缩后的文件名:";
                            string zipName; cin >> zipName;
                            DyHuffman ziphuf(path);
                            cout << "压缩中..." << endl;
                            ziphuf.zip(zipName);
                            cout << "压缩成功, 压缩率 = " << 100 * ziphuf.compressionRatio() << "%" << endl;
                            break;
                        }
                        case 2:
                        {
                            cout << "\n请选择要解压的文件:" << endl;
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "请务必选择文件！" << endl;
                                break;
                            }
                            cout << "请输入解压后的文件名(包括后缀):";
                            string unzipName; cin >> unzipName;
                            DyHuffman unziphuf(path);
                            cout << "解压中..." << endl;
                            unziphuf.unzip(unzipName);
                            cout << "解压成功." << endl;
                            break;
                        }
                        case 3:
                        {
                            flag1 = false;
                            break;
                        }
                        case 4:
                            return 0;
                        default:
                            cout << "输入错误,请重新选择!" << endl;
                    }
                    if (!flag1)break;
                    cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                    cout << "请选择:"; cin >> choice;
                }
                break;
            }
            case 4:
                return 0;
            default:
                cout << "输入错误，请重新选择!" << endl;
        }
        if (!flag) break;
        cout << "\n方式选择:\n1.普通哈夫曼\n2.K-叉哈夫曼\n3.动态哈夫曼\n4.退出" << endl;
        cout << "请选择:"; cin >> order;
    }
    return 0;
}