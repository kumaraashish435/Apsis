import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EarthView } from './earth-view';

describe('EarthView', () => {
  let component: EarthView;
  let fixture: ComponentFixture<EarthView>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [EarthView],
    }).compileComponents();

    fixture = TestBed.createComponent(EarthView);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
