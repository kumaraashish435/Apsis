import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SimulationEditor } from './simulation-editor';

describe('SimulationEditor', () => {
  let component: SimulationEditor;
  let fixture: ComponentFixture<SimulationEditor>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SimulationEditor],
    }).compileComponents();

    fixture = TestBed.createComponent(SimulationEditor);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
