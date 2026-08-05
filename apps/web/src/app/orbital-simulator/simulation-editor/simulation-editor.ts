import { HttpErrorResponse } from '@angular/common/http';
import { Component, OnInit, inject, signal } from '@angular/core';
import { FormBuilder, ReactiveFormsModule, Validators } from '@angular/forms';
import { ActivatedRoute, Router, RouterLink } from '@angular/router';
import { SaveSimulationRequest, SimulationInputType } from '../simulations/simulation.models';
import { SimulationsService } from '../simulations/simulations.service';

@Component({
  selector: 'app-simulation-editor',
  imports: [ReactiveFormsModule, RouterLink],
  templateUrl: './simulation-editor.html',
  styleUrl: './simulation-editor.scss',
})
export class SimulationEditor implements OnInit {
  private readonly fb = inject(FormBuilder);
  private readonly simulations = inject(SimulationsService);
  private readonly route = inject(ActivatedRoute);
  private readonly router = inject(Router);

  protected readonly simulationId = signal<string | null>(null);
  protected readonly inputType = signal<SimulationInputType>('Tle');
  protected readonly submitting = signal(false);
  protected readonly loading = signal(false);
  protected readonly errorMessage = signal<string | null>(null);

  protected readonly form = this.fb.nonNullable.group({
    name: ['', [Validators.required]],
    tleLine1: [''],
    tleLine2: [''],
    semiMajorAxisKm: [7000],
    eccentricity: [0],
    inclinationDeg: [51.6],
    raanDeg: [0],
    argPerigeeDeg: [0],
    trueAnomalyDeg: [0],
    startTime: [toLocalDatetimeInputValue(new Date().toISOString()), [Validators.required]],
    endTime: [toLocalDatetimeInputValue(addHours(new Date(), 2).toISOString()), [Validators.required]],
    stepSeconds: [60, [Validators.required, Validators.min(1)]],
  });

  ngOnInit(): void {
    const id = this.route.snapshot.paramMap.get('id');
    if (!id) {
      return;
    }
    this.simulationId.set(id);
    this.loading.set(true);
    this.simulations.get(id).subscribe({
      next: (sim) => {
        this.inputType.set(sim.inputType);
        this.form.patchValue({
          name: sim.name,
          tleLine1: sim.tleLine1 ?? '',
          tleLine2: sim.tleLine2 ?? '',
          semiMajorAxisKm: sim.semiMajorAxisKm ?? 7000,
          eccentricity: sim.eccentricity ?? 0,
          inclinationDeg: sim.inclinationDeg ?? 0,
          raanDeg: sim.raanDeg ?? 0,
          argPerigeeDeg: sim.argPerigeeDeg ?? 0,
          trueAnomalyDeg: sim.trueAnomalyDeg ?? 0,
          startTime: toLocalDatetimeInputValue(sim.startTime),
          endTime: toLocalDatetimeInputValue(sim.endTime),
          stepSeconds: sim.stepSeconds,
        });
        this.loading.set(false);
      },
      error: () => {
        this.errorMessage.set('Could not load this simulation.');
        this.loading.set(false);
      },
    });
  }

  protected setInputType(type: SimulationInputType): void {
    this.inputType.set(type);
  }

  protected submit(): void {
    if (this.form.invalid || this.submitting()) {
      this.form.markAllAsTouched();
      return;
    }

    const v = this.form.getRawValue();
    const request: SaveSimulationRequest = {
      name: v.name,
      inputType: this.inputType(),
      startTime: new Date(v.startTime).toISOString(),
      endTime: new Date(v.endTime).toISOString(),
      stepSeconds: v.stepSeconds,
      ...(this.inputType() === 'Tle'
        ? { tleLine1: v.tleLine1, tleLine2: v.tleLine2 }
        : {
            semiMajorAxisKm: v.semiMajorAxisKm,
            eccentricity: v.eccentricity,
            inclinationDeg: v.inclinationDeg,
            raanDeg: v.raanDeg,
            argPerigeeDeg: v.argPerigeeDeg,
            trueAnomalyDeg: v.trueAnomalyDeg,
          }),
    };

    this.submitting.set(true);
    this.errorMessage.set(null);

    const id = this.simulationId();
    const save = id ? this.simulations.update(id, request) : this.simulations.create(request);
    save.subscribe({
      next: (sim) => this.router.navigate(['/simulator/simulations', sim.id, 'view']),
      error: (err: HttpErrorResponse) => {
        this.submitting.set(false);
        this.errorMessage.set(err.error?.message ?? 'Something went wrong. Please check your inputs.');
      },
    });
  }
}

function toLocalDatetimeInputValue(iso: string): string {
  const d = new Date(iso);
  const pad = (n: number) => String(n).padStart(2, '0');
  return `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())}T${pad(d.getHours())}:${pad(d.getMinutes())}`;
}

function addHours(date: Date, hours: number): Date {
  return new Date(date.getTime() + hours * 3600_000);
}
