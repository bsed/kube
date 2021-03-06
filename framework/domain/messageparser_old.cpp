/*
    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "messageparser.h"
#include "mimetreeparser/interface.h"

PartModel::PartModel(QSharedPointer<MimeTreeParser::MessagePart> partTree, std::shared_ptr<Parser> parser)
    : mPartTree(partTree)
    , mParser(parser)
{
}

QHash<int, QByteArray> PartModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Text] = "text";
    roles[IsHtml] = "isHtml";
    roles[IsHidden] = "isHidden";
    roles[IsEncrypted] = "isEncrypted";
    roles[IsAttachment] = "isAttachment";
    roles[HasContent] = "hasContent";
    roles[Type] = "type";
    roles[IsHidden] = "isHidden";
    return roles;
}

QModelIndex PartModel::index(int row, int column, const QModelIndex &parent) const
{
    // qDebug() << "index " << parent << row << column << mPartTree->subParts().size();
    if (!parent.isValid()) {
        if (row < mPartTree->subParts().size()) {
            auto part = mPartTree->subParts().at(row);
            return createIndex(row, column, part.data());
        }
    } else {
        auto part = static_cast<MimeTreeParser::MessagePart*>(parent.internalPointer());
        auto subPart = part->subParts().at(row);
        return createIndex(row, column, subPart.data());
    }
    return QModelIndex();
}

QVariant PartModel::data(const QModelIndex &index, int role) const
{
    // qDebug() << "Getting data for index";
    if (index.isValid()) {
        auto part = static_cast<MimeTreeParser::MessagePart*>(index.internalPointer());
        switch (role) {
            case Text: {
                // qDebug() << "Getting text: " << part->property("text").toString();
                // FIXME: we should have a list per part, and not one for all parts.
                auto text = part->property("htmlContent").toString();
                const auto rx = QRegExp("(src)\\s*=\\s*(\"|')(cid:[^\"']+)\\2");
                int pos = 0;
                while ((pos = rx.indexIn(text, pos)) != -1) {
                    const auto link = QUrl(rx.cap(3).toUtf8());
                    pos += rx.matchedLength();
                    const auto repl = mParser->getPart(link);
                    if (!repl) {
                        continue;
                    }
                    const auto content = repl->content();
                    if(content.size() < 1) {
                        continue;
                    }
                    const auto mailMime = content.first()->mailMime();
                    const auto mimetype = mailMime->mimetype().name();
                    if (mimetype.startsWith("image/")) {
                        const auto data = content.first()->content();
                        text.replace(rx.cap(0), QString("src=\"data:%1;base64,%2\"").arg(mimetype, QString::fromLatin1(data.toBase64())));
                    }
                }
                return text;
            }
            case IsAttachment:
                return part->property("attachment").toBool();
            case IsEncrypted:
                return part->property("isEncrypted").toBool();
            case IsHtml:
                return part->property("isHtml").toBool();
            case HasContent:
                return !part->property("htmlContent").toString().isEmpty();
            case Type:
                return part->metaObject()->className();
            case IsHidden:
                return false;
                //return part->property("isHidden").toBool();

        }
    }
    return QVariant();
}

QModelIndex PartModel::parent(const QModelIndex &index) const
{
    // qDebug() << "parent " << index;
    if (index.isValid()) {
        auto part = static_cast<MimeTreeParser::MessagePart*>(index.internalPointer());
        auto parentPart = static_cast<MimeTreeParser::MessagePart*>(part->parentPart());
        auto row = 0;//get the parents parent to find the index
        if (!parentPart) {
            parentPart = mPartTree.data();
        }
        int i = 0;
        for (const auto &p : parentPart->subParts()) {
            if (p.data() == part) {
                row = i;
                break;
            }
            i++;
        }
        return createIndex(row, index.column(), parentPart);
    }
    return QModelIndex();
}

int PartModel::rowCount(const QModelIndex &parent) const
{
    // qDebug() << "Row count " << parent;
    if (!parent.isValid()) {
        // qDebug() << "Row count " << mPartTree->subParts().size();
        return mPartTree->subParts().size();
    } else {
        auto part = static_cast<MimeTreeParser::MessagePart*>(parent.internalPointer());
        if (part) {
            return part->subParts().size();
        }
    }
    return 0;
}

int PartModel::columnCount(const QModelIndex &parent) const
{
    // qDebug() << "Column count " << parent;
    return 1;
}

