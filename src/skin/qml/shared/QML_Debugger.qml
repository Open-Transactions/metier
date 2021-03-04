pragma Singleton
import QtQuick 2.0

//-----------------------------------------------------------------------------
// Tools for quick debugging Object avaliable properties and functions.
Item {

  //----------------------
  // List Object all
  function listEverything(item) {
    for (var prop in item) {
      console.log(prop + ":" + item[prop]);
    }
  }

  //----------------------
  // List all properties
  function listProperties(item) {
    for (var prop in item) {
      if( typeof item[prop] != "function" ) {
        if(prop != "objectName") console.log(prop + ":" + item[prop]);
      }
    }
  }

  //----------------------
  // list all functions
  function listFunctions(item) {
    for (var prop in item) {
      if( typeof item[prop] === "function" ) {
        if(prop != "objectName") console.log(prop + ":" + item[prop]);
      }
    }
  }

  //----------------------
  // Check if has function
  function hasFunction(item, function_name) {
    for (var prop in item) {
      if( typeof item[prop] === "function" && prop == function_name) {
        return true
      }
    }
    return false
  }

  //----------------------
  // Itterate threw the Components children.
  function listModelChildren(item, thorough = false) {
    console.log("Looking for children:")
    for (var i = 0; i < item.children.length; i++) {
      console.log("Child item " + i, item.children[i])
      if (thorough) {
        listEverything(item)
      }
    }
  }


}// end debug tools