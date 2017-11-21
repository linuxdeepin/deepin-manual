'use strict';

import * as marked from 'marked';
import * as urlparse from 'url-parse';


export class ManualAnchor {
  id: string;
  text: string;
  icon: string;
  smallIcon: string;
  children: ManualAnchor[];
}

export class Manual {

  // App manual name, like 'deepin-image-viewer'.
  manualName: string;

  // Original markdown text.
  markdown: string;

  html: string;

  // Relative path to app icon.
  appIcon: string;

  // App display name, like 'Deepin Image Viewer'.
  appName: string;

  // Header list.
  anchors: ManualAnchor[];

  // Text content of headers.
  headers: string[];

  constructor() { }

  static parseMarkdown(src: string): Manual {
    const m = new Manual();
    return m;
  }

}
