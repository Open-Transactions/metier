pragma Singleton

import QtQuick 2.15

import "qrc:/qml_shared"
//-----------------------------------------------------------------------------
// 'OTidentity'
// Manages current user NYM for asset displays provided from OpenTransactions.
/*
accountList: opentxs::ui::AccountListQt
activeNym: <mattercode_id string>
contactList: opentxs::ui::ContactListQt
nymList: opentxs::ui::NymListQt
nymTypet: QAbstractListModel
profile: opentxs::ui::ProfileQt

objectNameChanged:function()
activeNymChanged:function()
needNym:function()
setActiveNym:function()

getAccountActivityQML:function()
getAccountListQML:function()
getAccountStatusQML:function()
getAccountTreeQML:function()
getActivityThreadQML:function()
getContactListQML:function()
getNymListQML:function()
getNymTypeQML:function()
getProfileQML:function()
*/
//-----------------------------------------------------------------------------
QtObject {
  id: identityRootContext

  property bool isReady: true

  property var userIdentity_OTModel: undefined           // Nym identity manager and OT models provider.

  property var profile_OTModel: undefined                // Current Profile
  property var contactList_OTModel: undefined            // Current nym ContactList
  property var contactActivityThread_OTModel: undefined  // Current focused ActivityThread

  property var accountList_OTModel: undefined            // Current wallet AccountList
  property var focusedAccountStatus_OTModel: undefined   // Current focused AccountStatus
	property var focusedAccountActivity_OTModel: undefined // Current focused AccountActivity

  signal dataChanged() // Emitted when the nym has changed, informs update to pointer models.

  //-----------------------------------------------------------------------------
  // Change wallet identity:
  function changeActiveNym(to_mattercode_id) {
    identityRootContext.userIdentity_OTModel.setActiveNym(to_mattercode_id)
    identityRootContext.isReady = false
  }

  // Signal fired when there is no profile ready in the Identity manager.
  function requiresNymSelection() {
    console.log("Warning: Profile not yet created. identityRootContext.userIdentity_OTModel")
    identityRootContext.isReady = false
  }

  //----------------------
  // Called on linked signal to change in User idenity OT model:
  function activeNymChanged(to_mattercode_id) {
    identityRootContext.isReady = true
    // update attached models
    identityRootContext.profile_OTModel = userIdentity_OTModel.getProfileQML()
    identityRootContext.updateAccountModels()
    //debugger:
    console.log("identityRootContext Nym changed:", to_mattercode_id, isReady)
    // Fire signal to alert componets that are watching for changes to Identity
    identityRootContext.dataChanged()
  }

  // Update focused account asset models:
  function updateAccountModels() {
    identityRootContext.accountList_OTModel = userIdentity_OTModel.getAccountListQML()
    identityRootContext.dataChanged()
  }

  // Set the focusedAccountActivity_OTModel to the account ID provided:
  function setAccountActivityFocus(account_id) {
    identityRootContext.focusedAccountActivity_OTModel = userIdentity_OTModel.getAccountActivityQML(account_id)
    identityRootContext.focusedAccountStatus_OTModel = userIdentity_OTModel.getAccountStatusQML(account_id)
    //debugger:
    //console.log("identityRootContext setting AccountActivity focus:", account_id, focusedAccountActivity_OTModel)
    //QML_Debugger.listEverything(focusedAccountActivity_OTModel)
  }

  // Make sure the Profile model matches the current Identity set:
  function ensureProfileIsSet() {
    identityRootContext.profile_OTModel = userIdentity_OTModel.getProfileQML()
  }

  //----------------------
  // Set current nym identity's contact list model pointer:
  function checkContacts() {
    identityRootContext.contactList_OTModel = userIdentity_OTModel.getContactListQML()
  }

  // Set current contact ActivityThread model focus:
  function setContactActivityFocus(contact_id) {
    identityRootContext.contactActivityThread_OTModel = userIdentity_OTModel.getActivityThreadQML(contact_id)
    //debugger:
    //console.log("identityRootContext contact ActivityThread focus:", contact_id, contactActivityThread_OTModel)
    //QML_Debugger.listEverything(contactActivityThread_OTModel)
  }


  //-----------------------------------------------------------------------------
  // Link OT connections:
  Component.onCompleted: {
    // set identity
    identityRootContext.userIdentity_OTModel = api.identityManagerQML()
    identityRootContext.userIdentity_OTModel.activeNymChanged.connect(identityRootContext.activeNymChanged)
    identityRootContext.userIdentity_OTModel.needNym.connect(identityRootContext.requiresNymSelection)
    identityRootContext.ensureProfileIsSet()
    //debugger:
    console.log("identityRootContext starting up:", identityRootContext.userIdentity_OTModel)
    //QML_Debugger.listEverything(identityRootContext.userIdentity_OTModel)
  }

  // Disconnect any signals:
  Component.onDestruction: {
    identityRootContext.userIdentity_OTModel.activeNymChanged.connect(identityRootContext.activeNymChanged)
    identityRootContext.userIdentity_OTModel.needNym.connect(identityRootContext.requiresNymSelection)
  }

//-----------------------------------------------------------------------------
}//end 'identityRootContext'