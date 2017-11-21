import {Injectable} from '@angular/core';
import {Http} from '@angular/http';
import {Observable} from 'rxjs/Observable';
import 'rxjs/add/operator/catch';
import {AppError} from '../common/app-error';
import {NotFoundError} from '../common/not-found-error';
import {BadInputError} from '../common/bad-input-error';

@Injectable()
export class RequestsService {

  constructor(private http: Http) { }

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
