#pragma once


#include <QIcon>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStyle>
#include <QFile>
#include <iostream>
#include <QDebug>
#include <QLineEdit>

namespace Treexml
{

	class XmlSet
	{
	public:
		XmlSet()
		{
			/*
			QDomDocument doc;
			QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
			doc.appendChild(instruction);

			QDomElement root = doc.createElement("HInfoData");//创建根节点
			doc.appendChild(root);//添加根节点

			QDomElement strMac = doc.createElement("Mac");//创建元素节点
			root.appendChild(strMac);//添加元素节点到根节点
			QDomText strMacNodeText = doc.createTextNode(data._strMac);//创建元素文本
			strMac.appendChild(strMacNodeText);//添加元素文本到元素节点

			QFile file("./test.xml");
			if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
				return false;
			QTextStream out(&file);
			out.setCodec("UTF-8");
			doc.save(out, 4, QDomNode::EncodingFromTextStream);
			file.close();
			*/
		}

		void updateOnePara(QString paraId, QString paraKey, QString paraValue)
		{
			QFile file("sample.xml");
			if (!file.open(QFile::ReadOnly))
				return;

			QDomDocument doc;
			if (!doc.setContent(&file))
			{
				file.close();
				return;
			}
			file.close();

			QDomElement docElem = doc.documentElement();
			QDomNode n = docElem.firstChild();

			bool needNewId = true;
			bool needNewKey = true;

			while (!n.isNull()) 
			{
				QDomElement e = n.toElement();

				if (e.attributes().namedItem("id").nodeValue() == paraId)
				{
					needNewId = false;

					QDomNodeList list = e.childNodes();

					for (int i = 0; i< list.count(); i++) 
					{
						QDomNode node = list.at(i);
						if (node.isElement() && node.toElement().tagName() == paraKey)
						{
							needNewKey = false;

							QDomNode oldnode = node.toElement().firstChild();          
							node.firstChild().setNodeValue(paraValue);  
							QDomNode newnode = node.firstChild();
							node.replaceChild(newnode, oldnode);

							break;
						}
					}

					if (needNewKey)
					{
						QDomElement key = doc.createElement(paraKey);
						QDomText value = doc.createTextNode(paraValue);
						n.appendChild(key);
						key.appendChild(value);
					}

				}

				n = n.nextSibling();
			}

			if (needNewId && needNewKey)
			{
				QDomElement id = doc.createElement("item");
				id.setAttribute("id", paraId);
				
				QDomElement key = doc.createElement(paraKey);
				QDomText value = doc.createTextNode(paraValue);
				key.appendChild(value);
				id.appendChild(key);
				docElem.appendChild(id);
			}
			

			if (!file.open(QFile::WriteOnly | QFile::Truncate))
				return;

			QTextStream out_stream(&file);
			doc.save(out_stream, 4);
			file.close();
			
		}

		void deleteOnePara(QString paraId, QString paraKey)
		{
			QFile file("sample.xml");
			if (!file.open(QFile::ReadOnly))
				return;

			QDomDocument doc;
			if (!doc.setContent(&file))
			{
				file.close();
				return;
			}
			file.close(); 

			QDomElement root = doc.documentElement();
			QDomNodeList list = doc.elementsByTagName("item");
			QDomElement docElem = doc.documentElement();
			QDomNode n = docElem.firstChild();

			while (!n.isNull()) {
				QDomElement e = n.toElement();

				if (e.attributes().namedItem("id").nodeValue() == paraId)
				{
					QDomNodeList list = e.childNodes();

					for (int i = 0; i< list.count(); i++) {
						QDomNode node = list.at(i);
						if (node.isElement() && node.toElement().tagName() == paraKey)
						{
							e.removeChild(list.at(i));
							break;
						}
					}
				}

				n = n.nextSibling();
			}

			if (!file.open(QFile::WriteOnly | QFile::Truncate))
				return;
			
			QTextStream out_stream(&file);
			doc.save(out_stream, 4);
			file.close();			
		}

		bool getOneParaValue(QString paraId, QString paraKey, QString& paraValue)
		{
			QDomDocument doc("message");
			QFile file("sample.xml");
			if (!file.open(QIODevice::ReadOnly))
				return false;
			if (!doc.setContent(&file)) {
				file.close();
				return false;
			}
			file.close();

			QDomElement docElem = doc.documentElement();

			QDomNode n = docElem.firstChild();
			QDomElement root = docElem.firstChildElement();

			QDomNodeList item1 = root.elementsByTagName(paraId);

			while (!n.isNull()) {
				QDomElement e = n.toElement(); // try to convert the node to an element.
				qDebug() << qPrintable(e.tagName()) << qPrintable(e.attribute("id"));
				
				if (e.attributes().namedItem("id").nodeValue() == paraId)
				{					
					QDomNodeList list = e.childNodes();     

					for (int i = 0; i< list.count(); i++) {
						QDomNode node = list.at(i);                
						if (node.isElement() && node.toElement().tagName() == paraKey)
						{                   
							paraValue = node.toElement().text();
							return true;
						}            
					}        
				}
		
				n = n.nextSibling();
			}

			return false;
		}

		void loadXmlToTree(QTreeWidget* tw)
		{
			tw->clear();
			tw->setWindowTitle("QTreeWidget");
			tw->setHeaderLabels(QStringList() << "Key" << "Value");
			tw->setSortingEnabled(true);
			tw->header()->setSortIndicatorShown(true);
			tw->header()->setSortIndicator(0, Qt::AscendingOrder);

			QDomDocument doc("message");
			QFile file("sample.xml");
			if (!file.open(QIODevice::ReadOnly))
				return;
			if (!doc.setContent(&file)) {
				file.close();
				return;
			}
			file.close();

			QDomElement root = doc.documentElement();
			QDomNodeList list = doc.elementsByTagName("item");
			QDomElement docElem = doc.documentElement();
			QDomNode n = docElem.firstChild();

			while (!n.isNull()) {
				QDomElement e = n.toElement();
				
				QTreeWidgetItem* idItem = new QTreeWidgetItem(QStringList() << e.attribute("id"));
				idItem->setIcon(0, QIcon(":/images/a.png"));
				idItem->setCheckState(0, Qt::Checked);
				tw->addTopLevelItem(idItem);

				QDomNodeList list = e.childNodes();
				for (int i = 0; i< list.count(); i++) {
					QDomNode node = list.at(i);
					

					QStringList columItemList;
					QTreeWidgetItem *child;
					QString key, value;
					key += node.toElement().tagName();
					value += node.toElement().text();
					columItemList << key << value;
					child = new QTreeWidgetItem(columItemList);
					idItem->addChild(child);
				//	tw->setItemWidget(child, 1, new QLineEdit);
				}
				
				n = n.nextSibling();
			}	

			tw->show();
			tw->expandAll();
		}
};

	class Paraxml
	{
	public:
		Paraxml(QString path)
		{
			pFile = make_shared<QFile>(path);
			
			if (!pFile->open(QFile::ReadWrite | QFile::Text))
			{
				qDebug() << "Error: Cannot read file " << path
					<< ": " << qPrintable(pFile->errorString());
				return;
			}

			mReader.setDevice(&*pFile);

			
			/*
			mWriter.setDevice(&(*pFile));
			mWriter.setAutoFormatting(true);
			mWriter.writeStartDocument();
			mWriter.writeStartElement("xml");
			mWriter.writeStartElement("round");
			mWriter.writeAttribute("id", "1111");
			mWriter.writeAttribute("tableid", "2222");
			//player1
			mWriter.writeStartElement("player1");
			mWriter.writeAttribute("framescore", "1111");
			mWriter.writeAttribute("points", "1111");
			mWriter.writeAttribute("break", "1111");
			mWriter.writeCharacters("Hello World");   //添加内容
			mWriter.writeEndElement();
			//player2
			mWriter.writeStartElement("player2");
			mWriter.writeAttribute("framescore", "2222");
			mWriter.writeAttribute("points", "2222");
			mWriter.writeAttribute("break", "2222");
			mWriter.writeEndElement();
			mWriter.writeEndElement();
			mWriter.writeEndElement();
			mWriter.writeEndDocument();
			*/
		}

		bool getOneParaValue(QString paraId, QString paraKey, QString& paraValue)
		{
			while (!mReader.atEnd())
			{

				if (mReader.isStartElement())
				{

					QXmlStreamAttributes attributes = mReader.attributes();
					if (attributes.hasAttribute("paraId"))
					{
						qDebug() << attributes.value("term").toString();
					}
					

					auto tName = mReader.name().toString();

					if (mReader.name().toString() == paraKey)
					{
						paraValue = mReader.readElementText();
						return true;
					}

				}
				
				mReader.readNext();
			}

			return false;
		}


	public:
		std::shared_ptr<QFile> pFile;
		QXmlStreamReader mReader;
		QXmlStreamWriter mWriter;
	};

	class Readerxml
	{
	public:
		QXmlStreamReader xml;
		QTreeWidget *treeWidget;

		QIcon folderIcon;
		QIcon bookmarkIcon;


		Readerxml(QTreeWidget *tree)
		{
			treeWidget = tree;
			QStyle *style = treeWidget->style();

			folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
				QIcon::Normal, QIcon::Off);
			folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
				QIcon::Normal, QIcon::On);
			bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));
		}

		bool read(QIODevice *device)
		{
			xml.setDevice(device);

			if (xml.readNextStartElement())  //
			{
				if (xml.name() == "xbel" && xml.attributes().value("version") == "1.0")
					readXBEL();
				else
					xml.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
			}

			return !xml.error();
		}

		QString errorString() const
		{
			return QObject::tr("%1\nLine %2, column %3")
				.arg(xml.errorString())
				.arg(xml.lineNumber())
				.arg(xml.columnNumber());
		}

		void readXBEL()
		{
			Q_ASSERT(xml.isStartElement() && xml.name() == "xbel");

			while (xml.readNextStartElement())
			{
				if (xml.name() == "folder")
					readFolder(0);
				else if (xml.name() == "bookmark")
					readBookmark(0);
				else if (xml.name() == "separator")
					readSeparator(0);
				else
					xml.skipCurrentElement();
			}
		}

		void readTitle(QTreeWidgetItem *item)
		{
			Q_ASSERT(xml.isStartElement() && xml.name() == "title");

			QString title = xml.readElementText();
			item->setText(0, title);
		}

		void readSeparator(QTreeWidgetItem *item)
		{
			Q_ASSERT(xml.isStartElement() && xml.name() == "separator");

			QTreeWidgetItem *separator = createChildItem(item);
			separator->setFlags(item->flags() & ~Qt::ItemIsSelectable);
			separator->setText(0, QString(30, 0xB7));
			xml.skipCurrentElement();
		}

		void readFolder(QTreeWidgetItem *item)
		{
			Q_ASSERT(xml.isStartElement() && xml.name() == "folder");

			QTreeWidgetItem *folder = createChildItem(item);
			bool folded = (xml.attributes().value("folded") != "no");
			treeWidget->setItemExpanded(folder, !folded);

			while (xml.readNextStartElement())
			{
				if (xml.name() == "title")
					readTitle(folder);
				else if (xml.name() == "folder")
					readFolder(folder);
				else if (xml.name() == "bookmark")
					readBookmark(folder);
				else if (xml.name() == "separator")
					readSeparator(folder);
				else
					xml.skipCurrentElement();
			}
		}

		void readBookmark(QTreeWidgetItem *item)
		{
			Q_ASSERT(xml.isStartElement() && xml.name() == "bookmark");

			QTreeWidgetItem *bookmark = createChildItem(item);
			bookmark->setFlags(bookmark->flags() | Qt::ItemIsEditable);
			bookmark->setIcon(0, bookmarkIcon);
			bookmark->setText(0, QObject::tr("Unknown title"));
			bookmark->setText(1, xml.attributes().value("href").toString());

			while (xml.readNextStartElement())
			{
				if (xml.name() == "title")
					readTitle(bookmark);
				else
					xml.skipCurrentElement();
			}
		}

		QTreeWidgetItem *createChildItem(QTreeWidgetItem *item)
		{
			QTreeWidgetItem *childItem;
			if (item)
			{
				childItem = new QTreeWidgetItem(item);
			}
			else
			{
				childItem = new QTreeWidgetItem(treeWidget);
			}
			childItem->setData(0, Qt::UserRole, xml.name().toString());
			return childItem;
		}
	};

	class Writerxml
	{
	public:
		QXmlStreamWriter xml;
		QTreeWidget *treeWidget;

		Writerxml(QTreeWidget *tree)
		{
			treeWidget = tree;
			xml.setAutoFormatting(true); 
		}

		bool writeFile(QIODevice *device)
		{
			xml.setDevice(device);

			xml.writeStartDocument();
			xml.writeDTD("<!DOCTYPE xbel>");
			xml.writeStartElement("xbel");
			xml.writeAttribute("version", "1.0");
			for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
				writeItem(treeWidget->topLevelItem(i));

			xml.writeEndDocument();
			return true;
		}

		void writeItem(QTreeWidgetItem *item)
		{
			QString tagName = item->data(0, Qt::UserRole).toString();
			if (tagName == "folder")
			{
				bool folded = !treeWidget->isItemExpanded(item);
				xml.writeStartElement(tagName);
				xml.writeAttribute("folded", folded ? "yes" : "no");//三目运算
				xml.writeTextElement("title", item->text(0));
				for (int i = 0; i<item->childCount(); ++i)
					writeItem(item->child(i));
				xml.writeEndElement();
			}
			else if (tagName == "bookmark")
			{
				xml.writeStartElement(tagName);
				if (!item->text(1).isEmpty())
					xml.writeAttribute("href", item->text(1));
				xml.writeTextElement("title", item->text(0));
				xml.writeEndElement();
			}
			else if (tagName == "separator")
			{
				xml.writeEmptyElement(tagName);
			}
		}
	};
};