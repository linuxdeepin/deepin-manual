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
      .subscribe(html => {
        console.log('html:', html);
        this.manualContent = html;
        const iframe = document.querySelector('iframe');
        iframe.srcdoc = html;
        // iframe.setAttribute('srcdoc', iframe.srcdoc);
      });
  }

}
