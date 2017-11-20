import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';


import { AppComponent } from './app.component';
import { SearchComponent } from './search/search.component';
import { OverviewComponent } from './overview/overview.component';
import { ManualComponent } from './manual/manual.component';
import { ManualNavComponent } from './manual-nav/manual-nav.component';
import { ManualContentComponent } from './manual-content/manual-content.component';


@NgModule({
  declarations: [
    AppComponent,
    SearchComponent,
    OverviewComponent,
    ManualComponent,
    ManualNavComponent,
    ManualContentComponent
  ],
  imports: [
    BrowserModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
