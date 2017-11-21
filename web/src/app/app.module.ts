import {BrowserModule} from '@angular/platform-browser';
import {ErrorHandler, NgModule} from '@angular/core';

import {AppComponent} from './app.component';
import {SearchComponent} from './search/search.component';
import {OverviewComponent} from './overview/overview.component';
import {ManualComponent} from './manual/manual.component';
import {RequestsService} from './services/requests.service';
import {AppErrorHandler} from './common/app-error-handler';
import {ManualService} from './services/manual.service';
import {HttpClientModule} from '@angular/common/http';

@NgModule({
  declarations: [
    AppComponent,
    SearchComponent,
    OverviewComponent,
    ManualComponent,
  ],
  imports: [
    BrowserModule,
    HttpClientModule
  ],
  providers: [
    ManualService,
    RequestsService,
    { provide: ErrorHandler, useClass: AppErrorHandler },
  ],
  bootstrap: [
    AppComponent
  ]
})
export class AppModule { }
