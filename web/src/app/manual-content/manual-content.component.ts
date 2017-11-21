import { Component, OnInit } from '@angular/core';
import {ManualService} from '../services/manual.service';

@Component({
  selector: 'app-manual-content',
  templateUrl: './manual-content.component.html',
  styleUrls: ['./manual-content.component.css']
})
export class ManualContentComponent implements OnInit {

  manualContent: string;

  constructor(private service: ManualService) { }

  ngOnInit() {
    this.service.getManual('dde-file-manager', 'en_US')
      .subscribe(text => {
        console.log(text);
        this.manualContent = text;
        const iframe = document.querySelector('iframe');
        const html = this.service.markdownToHtml(text);
        console.log(iframe);
        iframe.srcdoc = html;
        iframe.setAttribute('srcdoc', iframe.srcdoc);
      });
  }

}
