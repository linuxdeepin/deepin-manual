import { TestBed, inject } from '@angular/core/testing';

import { ManualService } from './manual.service';

describe('ManualService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [ManualService]
    });
  });

  it('should be created', inject([ManualService], (service: ManualService) => {
    expect(service).toBeTruthy();
  }));
});
