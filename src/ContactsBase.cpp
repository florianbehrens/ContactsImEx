/*
 * ContactsBase.cpp
 *
 *  Created on: 29.08.2013
 *      Author: willy
 */

#include "ContactsBase.h"

bb::pim::contacts::ContactService ContactsBase::mContactService;
bool ContactsBase::mInitialized = false;
QVector<QString> ContactsBase::mAttributeKinds;
QVector<QString> ContactsBase::mAttributeSubKinds;
QString ContactsBase::postalAddressAttributePrefix("PostalAddress");

ContactsBase::ContactsBase()
  : mMergePolicy(KeepExisting)
{
	if (!mInitialized) {
		mInitialized = true;
		mAttributeKinds.reserve(27);
		mAttributeKinds <<
			"Invalid" <<
			"Phone" <<
			"Fax" <<
			"Pager" <<
			"Email" <<
			"Website" <<
			"Feed" <<
			"Profile" <<
			"Family" <<
			"Person" <<
			"Date" <<
			"Group" <<
			"Name" <<
			"StockSymbol" <<
			"Ranking" <<
			"OrganizationAffiliation" <<
			"Education" <<
			"Note" <<
			"InstantMessaging" <<
			"VideoChat" <<
			"ConnectionCount" <<
			"Hidden" <<
			"Biography" <<
			"Sound" <<
			"Notification" <<
			"MessageSound" <<
			"MessageNotification";
		mAttributeSubKinds.reserve(98);
		mAttributeSubKinds <<
			"Invalid" <<
			"Other" <<
			"Home" <<
			"Work" <<
			"PhoneMobile" <<
			"FaxDirect" <<
			"Blog" <<
			"WebsiteResume" <<
			"WebsitePortfolio" <<
			"WebsitePersonal" <<
			"WebsiteCompany" <<
			"ProfileFacebook" <<
			"ProfileTwitter" <<
			"ProfileLinkedIn" <<
			"ProfileGist" <<
			"ProfileTungle" <<
			"FamilySpouse" <<
			"FamilyChild" <<
			"FamilyParent" <<
			"PersonManager" <<
			"PersonAssistant" <<
			"DateBirthday" <<
			"DateAnniversary" <<
			"GroupDepartment" <<
			"NameGiven" <<
			"NameSurname" <<
			"Title" <<
			"NameSuffix" <<
			"NameMiddle" <<
			"NameNickname" <<
			"NameAlias" <<
			"NameDisplayName" <<
			"NamePhoneticGiven" <<
			"NamePhoneticSurname" <<
			"StockSymbolNyse" <<
			"StockSymbolNasdaq" <<
			"StockSymbolTse" <<
			"StockSymbolLse" <<
			"StockSymbolTsx" <<
			"RankingKlout" <<
			"RankingTrstRank" <<
			"OrganizationAffiliationName" <<
			"OrganizationAffiliationPhoneticName" <<
			"OrganizationAffiliationTitle" <<
			"StartDate" <<
			"EndDate" <<
			"OrganizationAffiliationDetails" <<
			"EducationInstitutionName" <<
			"EducationStartDate" <<
			"EducationEndDate" <<
			"EducationDegree" <<
			"EducationConcentration" <<
			"EducationActivities" <<
			"EducationNotes" <<
			"InstantMessagingBbmPin" <<
			"InstantMessagingAim" <<
			"InstantMessagingAliwangwang" <<
			"InstantMessagingGoogleTalk" <<
			"InstantMessagingSametime" <<
			"InstantMessagingIcq" <<
			"InstantMessagingIrc" <<
			"InstantMessagingJabber" <<
			"InstantMessagingMsLcs" <<
			"InstantMessagingMsn" <<
			"InstantMessagingQq" <<
			"InstantMessagingSkype" <<
			"InstantMessagingYahooMessenger" <<
			"InstantMessagingYahooMessengerJapan" <<
			"VideoChatBbPlaybook" <<
			"HiddenLinkedIn" <<
			"HiddenFacebook" <<
			"HiddenTwitter" <<
			"ConnectionCountLinkedIn" <<
			"ConnectionCountFacebook" <<
			"ConnectionCountTwitter" <<
			"HiddenChecksum" <<
			"HiddenSpeedDial" <<
			"BiographyFacebook" <<
			"BiographyTwitter" <<
			"BiographyLinkedIn" <<
			"SoundRingtone" <<
			"SimContactType" <<
			"EcoID" <<
			"Personal" <<
			"StockSymbolAll" <<
			"NotificationVibration" <<
			"NotificationLED" <<
			"MessageNotificationVibration" <<
			"MessageNotificationLED" <<
			"MessageNotificationDuringCall" <<
			"VideoChatPin" <<
			"NamePrefix" <<
			"Business" <<
			"ProfileSinaWeibo" <<
			"HiddenSinaWeibo" <<
			"ConnectionCountSinaWeibo" <<
			"BiographySinaWeibo" <<
			"DeviceInfo" <<
			"BBMNotificationTone" <<
			"BBMNotificationVibration" <<
			"BBMNotificationLED" <<
			"BBMNotificationInstantPreview" <<
			"HiddenJoynNumbers" <<
			"HiddenJoynInfo";
	}
}

ContactsBase::MergePolicy ContactsBase::mergePolicy() const
{
	return mMergePolicy;
}

void ContactsBase::setMergePolicy(MergePolicy arg)
{
	if (mMergePolicy != arg) {
		mMergePolicy = arg;
		emit mergePolicyChanged(mMergePolicy);
	}
}
