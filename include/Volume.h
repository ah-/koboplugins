#ifndef __VOLUME_H__
#define __VOLUME_H__

#include "Content.h"

class Volume : public Content {
public:
    Volume();
    Volume(QString&, QString&);
    Volume(Volume const&);
    virtual ~Volume();

    QString& getMimeType() const;
    QString& getId() const;
    QString& getTitle() const;
    bool isSideLoaded() const;


//    Volume::volumeWillBeDeleted(VolumeData*)
//    Volume::getContentType() const
//    Volume::getLanguage() const
//    Volume::getImageId() const
//    Volume::isQueuedForDownload() const
//    Volume::isOpen() const
//    Volume::isNew() const
//    Volume::isNewsOrMagazine() const
//    Volume::getUserID() const
//    Volume::isDownloaded() const
//    Volume::save(QString const&)
//    Volume::isFeedbackTypeSynced() const
//    Volume::setFeedbackType(FeedbackType)
//    Volume::setAttributes(QMap<QString, QVariant> const&)
//    Volume::getBookId() const
//    Volume::getReadStatus() const
//    Volume::getDbRetrieveFields()
//    Volume::setAttribute(QString const&, QVariant const&)
//    Volume::setUserKey(QString const&)
//    Volume::getChapterIdBookmarked() const
//    Volume::getDateLastReadString() const
//    Volume::getPublicationId() const
//    Volume::getDate() const
//    Volume::setDownloaded(bool)
//    Volume::setUserID(QString const&)
//    Volume::setExpirationStatus(int)
//    Volume::setBookId(QString const&)
//    Volume::isEmpty() const
//    Volume::setAttributes(QHash<QString, QString> const&)
//    Volume::getAccessibility() const
//    Volume::isValid() const
//    Volume::getNumShortcovers() const
//    Volume::setNumShortcovers(int)
//    Volume::getExpirationStatus() const
//    Volume::epubSize() const
//    Volume::setEpubSize(int)
//    Volume::isRepublished() const
//    Volume::setRepublished(bool)
//    Volume::isIncomplete() const
//    Volume::setIncomplete(bool)
//    Volume::getPercentRead() const
//    Volume::setReadStatus(int)
//    Volume::setISBN(QString const&)
//    Volume::setPageProgressDirection(QString)
//    Volume::setPublicationId(QString const&)
//    Volume::setLanguage(QString const&)
//    Volume::setAdobeLocation(QString const&)
//    Volume::setContentURL(QString const&)
//    Volume::setChapterIdBookmarked(QString const&)
//    Volume::setShelfData(ShelfContent const&)
//    Volume::isHelpVolume(QString)
//    Volume::isFullBookAccessible()
//    Volume::isOmf()
//    Volume::isFixedLayout()
//    Volume::epubType() const
//    Volume::setEpubType(int)
//    Volume::setQueuedForDownload(bool)
//    Volume::setNumPages(int)
//    Volume::getNumPages() const
//    Volume::setAccessibility(AccessibilityState)
//    Volume::setImageId(QString const&)
//    Volume::setPublisher(QString const&)
//    Volume::setAttribution(QString const&)
//    Volume::setMonetization(MonetizationState)
//    Volume::setDescription(QString const&)
//    Volume::setPercentRead(int)
//    Volume::setVolumeIndex(int)
//    Volume::getVolumeIndex() const
//    Volume::getBookmarkWordOffset() const
//    Volume::getParagraphBookmarked() const
//    Volume::setBookmarkWordOffset(int)
//    Volume::setParagraphBookmarked(int)
//    Volume::setFirstTimeReading(bool)
//    Volume::setContentType(QString const&)
//    Volume::setTitle(QString const&)
//    Volume::getSyncTime() const
//    Volume::getDateLastRead() const
//    Volume::setDate(QString const&)
//    Volume::setMimeType(QString const&)
//    Volume::isClosed() const
//    Volume::isFinished() const
//    Volume::monetization() const
//    Volume::isMagazine() const
//    Volume::isNewspaper() const
//    Volume::isFirstTimeReading() const
//    Volume::isSocialEnabled() const
//    Volume::setId(QString const&)
//    Volume::isMissingMetadata() const
//    Volume::setCurrencyCode(QString const&)
//    Volume::setTotalPrice(double)
//    Volume::totalPrice() const
//    Volume::setPriceBeforeTax(double)
//    Volume::priceBeforeTax() const
//    Volume::setContentCategory(ContentCategory)
//    Volume::contentCategory() const
//    Volume::setFeedbackTypeSynced(bool)
//    Volume::feedbackType() const
//    Volume::setAverageRating(double)
//    Volume::averageRating() const
//    Volume::getDbValues() const
//    Volume::getTableName()
//    Volume::convertToDBDate(QString const&)
//    Volume::convertToDBDateFormat(QString const&)
//    Volume::currencyCode() const
//    Volume::shelfData() const
//    Volume::getContentURL() const
//    Volume::getISBN() const
//    Volume::pageProgressDirection() const
//    Volume::getPublisher() const
//    Volume::getAdobeLocation() const
//    Volume::getUserKey() const
//    Volume::getPlainTextDescription() const
//    Volume::getAttribution() const
//    Volume::getDescription() const
//    Volume::getSyncTimeString() const
//    Volume::setSyncTime(QDateTime const&)
//    Volume::setDateLastRead(QDateTime const&)
//    Volume::isInternetArchive()

};

#endif
