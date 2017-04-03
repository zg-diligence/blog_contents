#include "OpenFile.h"
#include "2_Huffman.h"
#include "K_Huffman.h"
#include "Dy_FGK.h"
#include "Dy_Vitter.h"

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
 
                KHuffman<16> huf(text);
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
                cout << "\n算法选择:\n  1.FGK算法\n  2.Vitter算法\n  3.返回上一级\n  4.退出" << endl;
                cout << "请选择:"; int choice; cin >> choice;
                bool flag1 = true;
                while (flag1)
                {
                    switch (choice)
                    {
                    case 1:
                    {
                        cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                        cout << "请选择:"; int choice2; cin >> choice2;
                        bool flag2 = true;
                        while (flag2)
                        {
                            switch (choice2)
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
                                FGK ziphuf(path);
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
                                FGK unziphuf(path);
                                cout << "解压中..." << endl;
                                unziphuf.unzip(unzipName);
                                cout << "解压成功." << endl;
                                break;
                            }
                            case 3:
                            {
                                flag2 = false;
                                break;
                            }
                            case 4:
                                return 0;
                            default:
                                cout << "输入错误,请重新选择!" << endl;
                            }
                            if (!flag2)break;
                            cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                            cout << "请选择:"; cin >> choice2;
                        }
                        break;
                    }
                    case 2:
                    {
                        cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                        cout << "请选择:"; int choice; cin >> choice;
                        bool flag3 = true;
                        while (flag3)
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
                                Vitter ziphuf(path);
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
                                Vitter unziphuf(path);
                                cout << "解压中..." << endl;
                                unziphuf.unzip(unzipName);
                                cout << "解压成功." << endl;
                                break;
                            }
                            case 3:
                            {
                                flag3 = false;
                                break;
                            }
                            case 4:
                                return 0;
                            default:
                                cout << "输入错误,请重新选择!" << endl;
                            }
                            if (!flag3)break;
                            cout << "\n功能选择:\n  1.压缩文件\n  2.解压文件\n  3.返回上一级\n  4.退出" << endl;
                            cout << "请选择:"; cin >> choice;
                        }
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
                    cout << "\n算法选择:\n  1.FGK算法\n  2.Vitter算法\n  3.返回上一级\n  4.退出" << endl;
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