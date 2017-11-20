import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ManualContentComponent } from './manual-content.component';

describe('ManualContentComponent', () => {
  let component: ManualContentComponent;
  let fixture: ComponentFixture<ManualContentComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ManualContentComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ManualContentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
