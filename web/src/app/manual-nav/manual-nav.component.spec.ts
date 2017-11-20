import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ManualNavComponent } from './manual-nav.component';

describe('ManualNavComponent', () => {
  let component: ManualNavComponent;
  let fixture: ComponentFixture<ManualNavComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ManualNavComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ManualNavComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
