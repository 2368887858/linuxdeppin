// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H
#define QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H

#include <functional>
#include <QKeyEvent>
#include <QMenu>
#include <QUrl>

#include "qcef_widgets_export.h"

class QCefWebPage;

// The manner in which a link click should be opened. These constants match
// their equivalents in Chromium's window_open_disposition.h and should not be
// renumbered.
enum class QCEF_WIDGETS_EXPORT QCefWindowOpenDisposition {
  UNKNOWN,
  CURRENT_TAB,
  SINGLETON_TAB,
  NEW_FOREGROUND_TAB,
  NEW_BACKGROUND_TAB,
  NEW_POPUP,
  NEW_WINDOW,
  SAVE_TO_DISK,
  OFF_THE_RECORD,
  IGNORE_ACTION
};

// Supported context menu type flags.
enum QCefContextMenuFlags {
  // No node is selected.
  QCM_FLAG_NONE = 0,
  // The top page is selected.
  QCM_FLAG_PAGE = 1 << 0,
  // A subframe page is selected.
  QCM_FLAG_FRAME = 1 << 1,
  // A link is selected.
  QCM_FLAG_LINK = 1 << 2,
  // A media node is selected.
  QCM_FLAG_MEDIA = 1 << 3,
  // There is a textual or mixed selection that is selected.
  QCM_FLAG_SELECTION = 1 << 4,
  // An editable element is selected.
  QCM_FLAG_EDITABLE = 1 << 5,
};


// Supported context menu media types.
enum QCefContextMenuMediaTypeFlags {
  // No special node is in context.
  QCM_MEDIATYPE_NONE,
  // An image node is selected.
  QCM_MEDIATYPE_IMAGE,
  // A video node is selected.
  QCM_MEDIATYPE_VIDEO,
  // An audio node is selected.
  QCM_MEDIATYPE_AUDIO,
  // A file node is selected.
  QCM_MEDIATYPE_FILE,
  // A plugin node is selected.
  QCM_MEDIATYPE_PLUGIN,
};

// Supported context menu media state bit flags.
enum QCefContextMenuMediaFlags {
  QCM_MEDIAFLAG_NONE = 0,
  QCM_MEDIAFLAG_ERROR = 1 << 0,
  QCM_MEDIAFLAG_PAUSED = 1 << 1,
  QCM_MEDIAFLAG_MUTED = 1 << 2,
  QCM_MEDIAFLAG_LOOP = 1 << 3,
  QCM_MEDIAFLAG_CAN_SAVE = 1 << 4,
  QCM_MEDIAFLAG_HAS_AUDIO = 1 << 5,
  QCM_MEDIAFLAG_HAS_VIDEO = 1 << 6,
  QCM_MEDIAFLAG_CONTROL_ROOT_ELEMENT = 1 << 7,
  QCM_MEDIAFLAG_CAN_PRINT = 1 << 8,
  QCM_MEDIAFLAG_CAN_ROTATE = 1 << 9,
};

// Supported context menu edit state bit flags.
enum QCefContextMenuEditFlags {
  QCM_EDITFLAG_NONE = 0,
  QCM_EDITFLAG_CAN_UNDO = 1 << 0,
  QCM_EDITFLAG_CAN_REDO = 1 << 1,
  QCM_EDITFLAG_CAN_CUT = 1 << 2,
  QCM_EDITFLAG_CAN_COPY = 1 << 3,
  QCM_EDITFLAG_CAN_PASTE = 1 << 4,
  QCM_EDITFLAG_CAN_DELETE = 1 << 5,
  QCM_EDITFLAG_CAN_SELECT_ALL = 1 << 6,
  QCM_EDITFLAG_CAN_TRANSLATE = 1 << 7,
};

struct QCefContextMenuParamsPrivate;

// Provides information about the context menu state.
class QCEF_WIDGETS_EXPORT QCefContextMenuParams {
 public:
  QCefContextMenuParams();

  ~QCefContextMenuParams();

  typedef QCefContextMenuFlags TypeFlags;
  typedef QCefContextMenuMediaTypeFlags MediaType;
  typedef QCefContextMenuMediaFlags MediaStateFlags;
  typedef QCefContextMenuEditFlags EditStateFlags;

  // Returns the X coordinate of the mouse where the context menu was invoked.
  // Coords are relative to the associated RenderView's origin.
  int getXCoord() const;

  // Returns the Y coordinate of the mouse where the context menu was invoked.
  // Coords are relative to the associated RenderView's origin.
  int getYCoord() const;

  // Returns flags representing the type of node that the context menu was
  // invoked on.
  TypeFlags getTypeFlags() const;

  // Returns the URL of the link, if any, that encloses the node that the
  // context menu was invoked on.
  QString getLinkUrl() const;

  // Returns the link URL, if any, to be used ONLY for "copy link address". We
  // don't validate this field in the frontend process.
  QString getUnfilteredLinkUrl() const;

  // Returns the source URL, if any, for the element that the context menu was
  // invoked on. Example of elements with source URLs are img, audio, and video.
  QString getSourceUrl() const;

  // Returns true if the context menu was invoked on an image which has
  // non-empty contents.
  bool hasImageContents() const;

  // Returns the title text or the alt text if the context menu was invoked on
  // an image.
  QString getTitleText() const;

  // Returns the URL of the top level page that the context menu was invoked on.
  QString getPageUrl() const;

  // Returns the URL of the subframe that the context menu was invoked on.
  QString getFrameUrl() const;

  // Returns the character encoding of the subframe that the context menu was
  // invoked on.
  QString getFrameCharset() const;

  // Returns the type of context node that the context menu was invoked on.
  MediaType getMediaType() const;

  // Returns flags representing the actions supported by the media element, if
  // any, that the context menu was invoked on.
  MediaStateFlags getMediaStateFlags() const;

  // Returns the text of the selection, if any, that the context menu was
  // invoked on.
  QString getSelectionText() const;

  // Returns true if the context menu was invoked on an editable node.
  bool isEditable() const;

  // Returns flags representing the actions supported by the editable node, if
  // any, that the context menu was invoked on.
  EditStateFlags getEditStateFlags() const;

 private:
  friend class QCefClientHandlerDelegate;
  QCefContextMenuParamsPrivate* p_ = nullptr;
};

// A wrapper of Cef context menu.
class QCEF_WIDGETS_EXPORT QCefContextMenu {
 public:
  QCefContextMenu();

  typedef std::function<void(QCefWebPage* page)> Callback;

  enum class ItemType {
    Item,
    Separator,
  };
  struct MenuItem {
    ItemType type;
    int id;
    QString label;
    bool enabled;
  };

  enum MenuIdRange {
    // All user-defined menu IDs should come between MENU_ID_USER_FIRST and
    // MENU_ID_USER_LAST to avoid overlapping the Chromium and CEF ID ranges
    // defined in the tools/gritsettings/resource_ids file.
        MENU_ID_USER_FIRST = 26500,
    MENU_ID_USER_LAST = 28500,
  };

  // Add a new menu item.
  // |id| shall be between MENU_ID_USER_FIRST and MENU_ID_USER_LAST.
  // |label| can be localized string.
  // |enabled| set menu item activated or not.
  // |callback| is triggered when this menu item is activated.
  void addItem(int id, const QString& label, bool enabled, Callback callback);

  // Add a menu separator.
  void addSeparator();

  // Clear all menu items.
  void clear();

  const QVector<MenuItem> items() const;

  const QMap<int, Callback> callbacks() const;

 private:
  QVector<MenuItem> items_;
  QMap<int, Callback> callbacks_;
};

// Implement this class to handle mouse/keyboard events in cef browser window.
class QCEF_WIDGETS_EXPORT QCefBrowserEventDelegate {
 public:
  // Return true to cancel the navigation or false to allow the navigation
  // to proceed. |url| is the target url to be navigated to.
  virtual bool onBeforeBrowse(const QUrl& url, bool is_redirect) = 0;

  // Construct and popup a QMenu as context menu of browser window.
  virtual void onBeforeContextMenu(QCefWebPage* web_page,
                                   QCefContextMenu* menu,
                                   const QCefContextMenuParams& params) = 0;

  // This method is called before a new popup browser is created.
  // |url| is the position new popup window should navigate.
  // Returns true to popup default popup window.
  virtual bool onBeforePopup(const QUrl& url,
                             QCefWindowOpenDisposition disposition) = 0;

  // Triggered before a key event is sent to renderer process.
  // NOTE(LiuLang): Current |event| does not map to Qt Key event, only native
  // key code is available.
  virtual void onPreKeyEvent(const QKeyEvent& event) = 0;
};


#endif  // QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H
