import { ComponentFixture, TestBed } from '@angular/core/testing';

import { Orbitcanvas } from './orbitcanvas';

describe('Orbitcanvas', () => {
  let component: Orbitcanvas;
  let fixture: ComponentFixture<Orbitcanvas>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [Orbitcanvas],
    }).compileComponents();

    fixture = TestBed.createComponent(Orbitcanvas);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
