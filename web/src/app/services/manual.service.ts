import {Injectable} from '@angular/core';
import {Observable, Subscribable} from 'rxjs/Observable';
import {BadInputError} from '../common/bad-input-error';
import {NotFoundError} from '../common/not-found-error';
import {AppError} from '../common/app-error';
import 'rxjs/add/operator/catch';
import * as marked from 'marked';
import {HttpClient} from '@angular/common/http';
import {Subscription} from 'rxjs/Subscription';

@Injectable()
export class ManualService {

  constructor(private http: HttpClient) { }

  getManual(manual: string, lang: string): Observable<string> {
    const url = this.getManualUrl(manual, lang);
    console.log(url);
    return this.http.get(url, {responseType: 'text'})
      .catch(error => this.handleError(error));
  }

  // Render markdown content to HTML.
  markdownToHtml(tpl: string): string {
    return marked(tpl);
  }

  private getManualUrl(manual: string, lang: string): string {
    return `http://localhost:4300/usr/share/dman/${manual}/${lang}/index.md`;
  }

  private handleError(error: Response) {
    console.log('handleError: ', error);
    if (error.status === 400) {
      return Observable.throw(new BadInputError(error));
    } else if (error.status === 404) {
      return Observable.throw(new NotFoundError(error));
    } else {
      return Observable.throw(new AppError(error));
    }
  }
}
