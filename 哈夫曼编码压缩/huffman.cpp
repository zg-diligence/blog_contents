#include "OpenFile.h"
#include "Huffman.h"
#include "K-Huffman.h"
#include "DyHuffman.h"

int main()
{
    cout << "��ʽѡ��:\n  1.��ͨ������\n  2.K-�������\n  3.��̬������\n  4.�˳�" << endl;
    cout << "��ѡ��:"; int order; cin >> order;
    bool flag = true;
    while (flag)
    {
        switch (order)
        {
            case 1:
            {
                cout << "\n����ѡ��:\n  1.ѹ���ļ�\n  2.��ѹ�ļ�\n  3.������һ��\n  4.�˳�" << endl;
                cout << "��ѡ��:"; int choice; cin >> choice;
                bool flag1 = true;
                while (flag1)
                {
                    switch (choice)
                    {
                        case 1:
                        {
                            cout << "\n��ѡ��Ҫѹ�����ļ�:\n";
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "�����ѡ���ļ���" << endl;
                                break;
                            }
                            cout << "������ѹ������ļ���:";
                            string zipName; cin >> zipName;
                            Huffman ziphuf(path);
                            cout << "ѹ����..." << endl;
                            ziphuf.zip(zipName);
                            cout << "ѹ���ɹ�, ѹ���� = " << 100 * ziphuf.compressionRatio() << "%" << endl;
                            break;
                        }
                        case 2:
                        {
                            cout << "\n��ѡ��Ҫ��ѹ���ļ�:" << endl;
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "�����ѡ���ļ���" << endl;
                                break;
                            }
                            cout << "�������ѹ����ļ���(������׺):";
                            string unzipName; cin >> unzipName;
                            Huffman unziphuf(path);
                            cout << "��ѹ��..." << endl;
                            unziphuf.unzip(unzipName);
                            cout << "��ѹ�ɹ�." << endl;
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
                            cout << "�������,������ѡ��!" << endl;
                    }
                    if (!flag1)break;
                    cout << "\n����ѡ��:\n  1.ѹ���ļ�\n  2.��ѹ�ļ�\n  3.������һ��\n  4.�˳�" << endl;
                    cout << "��ѡ��:"; cin >> choice;
                }
                break;
            }
            case 2:
            {
                cout << "\n��ѡ��Ҫѹ�����ļ�:" << endl;
                string path = openFile(L"D://text");
                if (path.empty())
                {
                    cout << "�����ѡ���ļ���" << endl;
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
                cout << "ѹ����..." << endl;
                huf.encode();
                cout << "ѹ�����." << endl;
                cout << "\n��ѹ��..." << endl;
                out << huf.decode();
                cout << "��ѹ���." << endl;

                in.close();
                out.close();
                break;
            }
            case 3:
            {
                cout << "\n����ѡ��:\n  1.ѹ���ļ�\n  2.��ѹ�ļ�\n  3.������һ��\n  4.�˳�" << endl;
                cout << "��ѡ��:"; int choice; cin >> choice;
                bool flag1 = true;
                while (flag1)
                {
                    switch (choice)
                    {
                        case 1:
                        {
                            cout << "\n��ѡ��Ҫѹ�����ļ�:\n";
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "�����ѡ���ļ���" << endl;
                                break;
                            }
                            cout << "������ѹ������ļ���:";
                            string zipName; cin >> zipName;
                            DyHuffman ziphuf(path);
                            cout << "ѹ����..." << endl;
                            ziphuf.zip(zipName);
                            cout << "ѹ���ɹ�, ѹ���� = " << 100 * ziphuf.compressionRatio() << "%" << endl;
                            break;
                        }
                        case 2:
                        {
                            cout << "\n��ѡ��Ҫ��ѹ���ļ�:" << endl;
                            string path = openFile(L"D://text");
                            if (path.empty())
                            {
                                cout << "�����ѡ���ļ���" << endl;
                                break;
                            }
                            cout << "�������ѹ����ļ���(������׺):";
                            string unzipName; cin >> unzipName;
                            DyHuffman unziphuf(path);
                            cout << "��ѹ��..." << endl;
                            unziphuf.unzip(unzipName);
                            cout << "��ѹ�ɹ�." << endl;
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
                            cout << "�������,������ѡ��!" << endl;
                    }
                    if (!flag1)break;
                    cout << "\n����ѡ��:\n  1.ѹ���ļ�\n  2.��ѹ�ļ�\n  3.������һ��\n  4.�˳�" << endl;
                    cout << "��ѡ��:"; cin >> choice;
                }
                break;
            }
            case 4:
                return 0;
            default:
                cout << "�������������ѡ��!" << endl;
        }
        if (!flag) break;
        cout << "\n��ʽѡ��:\n1.��ͨ������\n2.K-�������\n3.��̬������\n4.�˳�" << endl;
        cout << "��ѡ��:"; cin >> order;
    }
    return 0;
}