import { ComponentFixture, TestBed } from '@angular/core/testing';

import { Orbit } from './orbit';

describe('Orbit', () => {
  let component: Orbit;
  let fixture: ComponentFixture<Orbit>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [Orbit],
    }).compileComponents();

    fixture = TestBed.createComponent(Orbit);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
