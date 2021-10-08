#include "xmlutils.h"



XmlUtils::XmlUtils(QString p)
{
   path = p;
}

XmlUtils::~XmlUtils()
{

}

void XmlUtils::WriteXml()
{
       //打开或创建文件
       QFile file(path); //相对路径、绝对路径、资源路径都可以
       if(!file.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
           return;

       QDomDocument doc;
       //写入xml头部
       QDomProcessingInstruction instruction; //添加处理命令
       instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
       doc.appendChild(instruction);

       //根节点
       QDomElement root=doc.createElement("config");
       doc.appendChild(root);

       QDomText text;
       //添加元素listenpath
       text=doc.createTextNode("D:/");
       QDomElement listenpath=doc.createElement(LISTENPATH); //创建子元素
       listenpath.appendChild(text);
       root.appendChild(listenpath);


       //添加元素cmdpath
       text=doc.createTextNode("D:/");
       QDomElement cmdpath=doc.createElement(CMDPATH); //创建子元素
       cmdpath.appendChild(text);
       root.appendChild(cmdpath);

       //输出到文件
       QTextStream out_stream(&file);
       doc.save(out_stream,4); //缩进4格
       file.close();
}

QString XmlUtils::ReadXml(QString key)
{
       if(!map.isEmpty()){
           return map.find(key).value();
       }
      //  qDebug()<< "sss"<< map.find(key).value();
    //打开或创建文件
       QFile file(path); //相对路径、绝对路径、资源路径都行
       if(!file.open(QFile::ReadOnly))
           return "";

       QDomDocument doc;
       if(!doc.setContent(&file))
       {
           file.close();
            return "";
       }
       file.close();

       QDomElement root=doc.documentElement(); //返回根节点
   //    qDebug()<<root.nodeName();
       QDomNode node=root.firstChild(); //获得第一个子节点
       while (!node.isNull())
       {
           QString tagName = node.toElement().tagName();  //节点元素名称
           if (tagName.compare(LISTENPATH) == 0) //节点标记查找
           {
                map.insert(LISTENPATH,node.toElement().text());
      //          qDebug()<< node.toElement().text();//读取节点文本
           }
           else if (tagName.compare(CMDPATH) == 0)
           {
               map.insert(CMDPATH,node.toElement().text());
  //             qDebug()<< node.toElement().text();
           }
           node = node.nextSibling();//读取下一个兄弟节点
       }
        return map.find(key).value();
}


void XmlUtils::UpdateXml(QString key, QString val)
{
     qDebug()<<path;
      QFile file(path);
      if(!file.open(QFile::ReadOnly))
          return;
      QDomDocument doc;
      if(!doc.setContent(&file))
      {
          file.close();
          return;
      }
      file.close();

      QDomElement root=doc.documentElement(); //返回根节点
      qDebug()<<root.nodeName();
      QDomNode node=root.firstChild(); //获得第一个子节点
      while (!node.isNull())
      {
          QString tagName = node.toElement().tagName();  //节点元素名称
          if (tagName.compare(key) == 0) //节点标记查找
          {
               qDebug()<<"key="<<tagName;
              QDomNode oldnode = node.firstChild();     //标签之间的内容作为节点的子节点出现，得到原来的子节点
              node.firstChild().setNodeValue(val);   //用提供的value值来设置子节点的内容
              QDomNode newnode = node.firstChild();     //值修改过后
              node.replaceChild(newnode,oldnode);      //调用节点的replaceChild方法实现修改功能
              map.insert((key),val);
              break;
          }
          node = node.nextSibling();//读取下一个兄弟节点
      }


      if(!file.open(QFile::WriteOnly))
          return;
      //输出到文件
      QTextStream out_stream(&file);
      doc.save(out_stream,4); //缩进4格
      file.close();
}

