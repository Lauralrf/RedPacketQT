#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTextCodec>
#include <ctime>
#include <fstream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str1 =ui->number->text();   //读取文本
    QString str2 =ui->money->text();    //读取文本
    int number= str1.toInt();    //转化为整形
    int money= str2.toInt();
    int maxpacket=0,minpacket=money;

    ofstream fout("result.txt");
    fout<<"总金额："<<money<<endl;
    fout<<"总人数："<<number<<endl;

    //以下为c++源程序，略有调整
    struct packet       //包红包，建立单链表
    {
        int id;
        int gift;
        struct packet* p;
    };
    struct packet *head, *temp, *current;
    srand((unsigned) time(NULL));
    temp = new packet;
    head = temp;
    for (int i=1; i<number; i++)
    {
        temp->id = i;
        temp->gift = rand() % money;
        money = money - temp->gift;
        current = temp;
        temp = new packet;
        current->p = temp;
    }
    temp->id = number;
    temp->gift = money;
    temp->p = NULL;


    for (int i=0; i<number; i++)        //发红包，每派发一个就删除链表中相应节点
    {
        int n = rand()% (number-i);

        temp = head;
        for(int j=0;j<n;j++)
        {
            current = temp;
            temp = temp->p;
        }
        if (n>0)
            current->p = temp->p;
        else
            head = temp->p;

        QString str=QString::number(i);
        str = str +QTextCodec::codecForName("GB2312")->toUnicode("号红包，id号：");
        str = str +QString::number(temp->id);
        str = str +QTextCodec::codecForName("GB2312")->toUnicode("，金额：");
        str = str +QString::number(temp->gift);
        ui->textEdit->append(str);    //在窗口中输出结果
        fout<<i<<"号红包，id号："<<temp->id<<"，金额："<<temp->gift<<endl;

        if (maxpacket<temp->gift)
            maxpacket = temp->gift;
        if (minpacket>temp->gift)
            minpacket = temp->gift;

        delete temp;
    }

    ui->maxp->setText(QString::number(maxpacket));
    ui->minp->setText(QString::number(minpacket));
    fout<<" 欧 皇："<<maxpacket<<endl;
    fout<<" 非 酋："<<minpacket<<endl;
    fout.close();
}
