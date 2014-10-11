// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2013 Adam Roben <adam@roben.org>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-CHROMIUM file.

#ifndef BRIGHTRAY_BROWSER_INSPECTABLE_WEB_CONTENTS_IMPL_H_
#define BRIGHTRAY_BROWSER_INSPECTABLE_WEB_CONTENTS_IMPL_H_

#include "browser/inspectable_web_contents.h"

#include "browser/devtools_contents_resizing_strategy.h"
#include "browser/devtools_embedder_message_dispatcher.h"

#include "content/public/browser/devtools_client_host.h"
#include "content/public/browser/devtools_frontend_host.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/gfx/rect.h"

class PrefRegistrySimple;

namespace content {
class DevToolsAgentHost;
}

namespace brightray {

class InspectableWebContentsDelegate;
class InspectableWebContentsView;

class InspectableWebContentsImpl :
    public InspectableWebContents,
    public content::DevToolsFrontendHost::Delegate,
    public content::DevToolsClientHost,
    public content::WebContentsObserver,
    public content::WebContentsDelegate,
    public DevToolsEmbedderMessageDispatcher::Delegate {
 public:
  static void RegisterPrefs(PrefRegistrySimple* pref_registry);

  explicit InspectableWebContentsImpl(content::WebContents*);
  virtual ~InspectableWebContentsImpl();

  virtual InspectableWebContentsView* GetView() const OVERRIDE;
  virtual content::WebContents* GetWebContents() const OVERRIDE;

  virtual void ShowDevTools() OVERRIDE;
  virtual void CloseDevTools() OVERRIDE;
  virtual bool IsDevToolsViewShowing() OVERRIDE;

  // Return the last position and size of devtools window.
  gfx::Rect GetDevToolsBounds() const;
  void SaveDevToolsBounds(const gfx::Rect& bounds);

  virtual void SetDelegate(InspectableWebContentsDelegate* delegate) {
    delegate_ = delegate;
  }
  virtual InspectableWebContentsDelegate* GetDelegate() const {
    return delegate_;
  }

  content::WebContents* devtools_web_contents() {
    return devtools_web_contents_.get();
  }

 private:
  // DevToolsEmbedderMessageDispacher::Delegate

  virtual void ActivateWindow() OVERRIDE;
  virtual void CloseWindow() OVERRIDE;
  virtual void SetInspectedPageBounds(const gfx::Rect& rect) OVERRIDE;
  virtual void InspectElementCompleted() OVERRIDE;
  virtual void MoveWindow(int x, int y) OVERRIDE;
  virtual void SetIsDocked(bool docked) OVERRIDE;
  virtual void OpenInNewTab(const std::string& url) OVERRIDE;
  virtual void SaveToFile(const std::string& url,
                          const std::string& content,
                          bool save_as) OVERRIDE;
  virtual void AppendToFile(const std::string& url,
                            const std::string& content) OVERRIDE;
  virtual void RequestFileSystems() OVERRIDE;
  virtual void AddFileSystem() OVERRIDE;
  virtual void RemoveFileSystem(const std::string& file_system_path) OVERRIDE;
  virtual void UpgradeDraggedFileSystemPermissions(
      const std::string& file_system_url) OVERRIDE;
  virtual void IndexPath(int request_id,
                         const std::string& file_system_path) OVERRIDE;
  virtual void StopIndexing(int request_id) OVERRIDE;
  virtual void SearchInPath(int request_id,
                            const std::string& file_system_path,
                            const std::string& query) OVERRIDE;
  virtual void ZoomIn() OVERRIDE;
  virtual void ZoomOut() OVERRIDE;
  virtual void ResetZoom() OVERRIDE;

  // content::DevToolsClientHost:
  virtual void DispatchOnInspectorFrontend(const std::string& message) override;
  virtual void InspectedContentsClosing() override;
  virtual void ReplacedWithAnotherClient() override;

  // content::DevToolsFrontendHostDelegate:
  virtual void HandleMessageFromDevToolsFrontend(const std::string& message) override;
  virtual void HandleMessageFromDevToolsFrontendToBackend(const std::string& message) override;

  // content::WebContentsObserver:
  virtual void AboutToNavigateRenderView(content::RenderViewHost* render_view_host) override;
  virtual void DidFinishLoad(content::RenderFrameHost* render_frame_host,
                             const GURL& validated_url) OVERRIDE;
  virtual void WebContentsDestroyed() OVERRIDE;

  // content::WebContentsDelegate

  virtual bool AddMessageToConsole(content::WebContents* source,
                                   int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents*, const content::NativeWebKeyboardEvent&) OVERRIDE;
  virtual void CloseContents(content::WebContents* source) OVERRIDE;

  scoped_ptr<content::WebContents> web_contents_;
  scoped_ptr<content::WebContents> devtools_web_contents_;
  scoped_ptr<InspectableWebContentsView> view_;
  scoped_refptr<content::DevToolsAgentHost> agent_host_;
  scoped_ptr<content::DevToolsFrontendHost> frontend_host_;

  DevToolsContentsResizingStrategy contents_resizing_strategy_;
  gfx::Rect devtools_bounds_;

  scoped_ptr<DevToolsEmbedderMessageDispatcher> embedder_message_dispatcher_;

  InspectableWebContentsDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(InspectableWebContentsImpl);
};

}  // namespace brightray

#endif
