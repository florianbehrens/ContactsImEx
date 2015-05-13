/*
 * (c) Copyright Florian Behrens 2013-2015.
 *
 * This file is part of ContactsImEx.
 *
 * ContactsImEx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ContactsImEx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ContactsImEx.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ContactsBase.h"

bb::pim::contacts::ContactService ContactsBase::mContactService;
bool ContactsBase::mInitialized = false;
QVector<QString> ContactsBase::mAttributeKinds;
QVector<QString> ContactsBase::mAttributeSubKinds;
QString ContactsBase::postalAddressAttributePrefix("PostalAddress");

ContactsBase::ContactsBase()
  : mMergePolicy(KeepExisting),
    mRunning(false)
{
	if (!mInitialized) {
		mInitialized = true;
		mAttributeKinds.reserve(28);
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
			"MessageNotification" <<
			"SecureVoice";
		mAttributeSubKinds.reserve(119);
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
			"StartDate" <<
			"EndDate" <<
			"OrganizationAffiliationDetails" <<
			"EducationInstitutionName" <<
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
			"HiddenJoynInfo"  <<
			"BBMNotificationInstantPreview" <<
			"PhoneNotificationAllAlert" <<
			"MessageNotificationAllAlert" <<
			"MessageNotificationInstantPreview" <<
			"BBMNotificationAllAlert" <<
			"TextMessageNotificationAllAlert" <<
			"TextMessageNotificationInstantPreview" <<
			"TextMessageNotificationRingtone" <<
			"TextMessageNotificationVibration" <<
			"TextMessageNotificationLED" <<
			"OtherMessageNotificationAllAlert" <<
			"OtherMessageNotificationInstantPreview" <<
			"OtherMessageNotificationRingtone" <<
			"OtherMessageNotificationVibration" <<
			"OtherMessageNotificationLED" <<
			"SecureVoiceId" <<
			"HiddenInstantMessagingBbmId" <<
			"HiddenVoLTEVideo" <<
			"HiddenVoLTEVoice";
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

ContactsBase::RunStateActivator* ContactsBase::setRunning()
{
	return new RunStateActivator(this);
}

ContactsBase::RunStateActivator::RunStateActivator(ContactsBase *contactsBase)
: mContactsBase(contactsBase)
{
	contactsBase->mRunning = true;
	emit mContactsBase->runningChanged(mContactsBase->mRunning);
}

ContactsBase::RunStateActivator::~RunStateActivator()
{
	mContactsBase->mRunning = false;
	emit mContactsBase->runningChanged(mContactsBase->mRunning);
}
