#include "MusicModelItem.h"

#include "../globals/Globals.h"

MusicModelItem::MusicModelItem(MusicModelItem *parent) : QObject(0)
{
    m_parentItem = parent;
    m_artist = 0;
    m_album = 0;
}

MusicModelItem::~MusicModelItem()
{
    qDeleteAll(m_childItems);
}

MusicModelItem *MusicModelItem::child(int number)
{
    return m_childItems.value(number);
}

int MusicModelItem::childCount() const
{
    return m_childItems.count();
}

int MusicModelItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<MusicModelItem*>(this));

    return 0;
}

int MusicModelItem::columnCount() const
{
    return 1;
}

QVariant MusicModelItem::data(int column) const
{
    Q_UNUSED(column);
    switch (column) {
    case 2:
        if (m_artist)
            return m_artist->hasChanged();
        else if (m_album)
            return m_album->hasChanged();
        break;
    default:
        if (m_artist)
            return m_artist->name();
        else if (m_album)
            return m_album->title();

    }

    return QVariant();
}

MusicModelItem *MusicModelItem::appendChild(Artist *artist)
{
    MusicModelItem *item = new MusicModelItem(this);
    item->setArtist(artist);
    artist->setModelItem(item);
    m_childItems.append(item);
    return item;
}

MusicModelItem *MusicModelItem::appendChild(Album *album)
{
    MusicModelItem *item = new MusicModelItem(this);
    item->setAlbum(album);
    album->setModelItem(item);
    m_childItems.append(item);
    connect(album, SIGNAL(sigChanged(Album*)), this, SLOT(onAlbumChanged(Album*)), Qt::UniqueConnection);
    return item;
}

MusicModelItem *MusicModelItem::parent()
{
    return m_parentItem;
}

bool MusicModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

void MusicModelItem::setArtist(Artist *artist)
{
    m_artist = artist;
}

void MusicModelItem::setAlbum(Album *album)
{
    m_album = album;
}

Artist *MusicModelItem::artist()
{
    return m_artist;
}

Album *MusicModelItem::album()
{
    return m_album;
}

int MusicModelItem::type()
{
    if (m_artist)
        return TypeArtist;
    else if (m_album)
        return TypeAlbum;

    return -1;
}

void MusicModelItem::onAlbumChanged(Album *album)
{
    emit sigIntChanged(this, album->modelItem());
}