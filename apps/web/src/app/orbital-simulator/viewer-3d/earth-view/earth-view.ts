import {
  AfterViewInit,
  Component,
  ElementRef,
  NgZone,
  OnDestroy,
  ViewChild,
  effect,
  input,
} from '@angular/core';
import * as THREE from 'three';
import type { OrbitControls } from 'three-stdlib';

import { createScene } from './scene/scene';
import { createCamera, fitCameraToBoundingSphere } from './camera/camera';
import { createRenderer } from './renderer/renderer';
import { createOrbitControls } from './camera/orbit-controls';
import { OrbitControlsGate } from './input/orbit-controls-gate';
import { ResizeHandler } from './renderer/resize';
import { AnimationLoop } from './renderer/animation-loop';
import { createEarth } from './earth/earth';
import { createLights } from './earth/lights';
import { Simulation } from './simulation/simulation';
import { SatelliteEntry, SatelliteManager } from './satellite/satellite-manager';
import { EARTH_AXIAL_TILT_DEG, EARTH_SIDEREAL_DAY_SECONDS, SCENE_EARTH_RADIUS } from './utils/constants';
import { disposeObject3D } from './utils/helpers';
import { kmToScene } from './utils/math';

@Component({
  selector: 'app-earth-view',
  imports: [],
  templateUrl: './earth-view.html',
  styleUrl: './earth-view.scss',
})
export class EarthView implements AfterViewInit, OnDestroy {
  @ViewChild('canvasContainer')
  container!: ElementRef<HTMLDivElement>;

  /** Real propagated tracks to render — set once GET .../propagate resolves. See ../viewer-3d.ts. */
  readonly satellites = input<SatelliteEntry[]>([]);

  private scene!: THREE.Scene;
  private camera!: THREE.PerspectiveCamera;
  private renderer!: THREE.WebGLRenderer;
  private controls!: OrbitControls;
  private satelliteManager?: SatelliteManager;
  private sceneReady = false;

  // Two nested groups, not one: earthTilt is set once and never touched
  // again; earthSpin is its CHILD and gets rotated every frame. Because
  // earthSpin's local Y axis is inherited from its already-tilted parent,
  // spinning it can only ever rotate around the tilted axis — mixing a
  // constant tilt and a per-frame spin into a single group's Euler rotation
  // (two components of one Euler) is order-fragile and was producing a
  // level/untitled-looking spin instead.
  private earthTilt!: THREE.Group;
  private earthSpin!: THREE.Group;

  private animationLoop!: AnimationLoop;
  private resizeHandler!: ResizeHandler;
  private orbitControlsGate!: OrbitControlsGate;

  // Objects OrbitControls is allowed to engage on — see OrbitControlsGate.
  // Push satellite groups in here too once they're wired up, so drag/scroll
  // works on them as well as Earth.
  private readonly interactiveObjects: THREE.Object3D[] = [];

  private readonly simulation = new Simulation(600);

  constructor(private readonly zone: NgZone) {
    effect(() => {
      const entries = this.satellites();
      if (this.sceneReady) {
        this.applySatellites(entries);
      }
    });
  }

  ngAfterViewInit(): void {
    const el = this.container.nativeElement;

    this.scene = createScene();
    this.camera = createCamera(el.clientWidth / el.clientHeight);
    this.renderer = createRenderer(el);
    this.controls = createOrbitControls(this.camera, this.renderer.domElement);

    this.earthTilt = new THREE.Group();
    this.earthTilt.rotation.z = THREE.MathUtils.degToRad(EARTH_AXIAL_TILT_DEG);
    this.scene.add(this.earthTilt);

    this.earthSpin = new THREE.Group();
    this.earthSpin.add(createEarth());
    this.earthTilt.add(this.earthSpin);
    this.interactiveObjects.push(this.earthTilt);

    const { ambient, sun } = createLights();
    this.scene.add(ambient, sun);

    this.orbitControlsGate = new OrbitControlsGate(
      this.controls,
      this.camera,
      this.renderer.domElement,
      () => this.interactiveObjects,
    );

    this.resizeHandler = new ResizeHandler(el, this.camera, this.renderer, () =>
      this.controls.update(),
    );

    // Satellite groups live under earthTilt (the constant-tilt frame), not
    // earthSpin (which additionally rotates every frame to show Earth's own
    // rotation) — orbit positions are in the inertial ECI/TEME frame, so they
    // must NOT spin along with the ground beneath them.
    this.satelliteManager = new SatelliteManager(this.earthTilt);
    this.sceneReady = true;
    this.applySatellites(this.satellites());

    // Run the render loop outside Angular's zone — requestAnimationFrame at
    // ~60fps would otherwise trigger a full change-detection pass on every
    // single frame for as long as this component is alive.
    this.zone.runOutsideAngular(() => {
      this.animationLoop = new AnimationLoop((deltaSeconds) => this.onFrame(deltaSeconds));
      this.animationLoop.start();
    });
  }

  private applySatellites(entries: SatelliteEntry[]): void {
    if (!this.satelliteManager) {
      return;
    }

    this.satelliteManager.dispose();
    for (const entry of entries) {
      if (entry.states.length >= 2) {
        this.satelliteManager.addSatellite(entry.name, entry.states);
      }
    }
    this.interactiveObjects.push(...this.satelliteManager.getGroups());

    if (entries.length > 0) {
      const maxRadiusScene = Math.max(
        ...entries.flatMap((e) => e.states.map((s) => kmToScene(s.xKm, s.yKm, s.zKm).length())),
      );
      fitCameraToBoundingSphere(
        this.camera,
        this.controls,
        new THREE.Sphere(new THREE.Vector3(0, 0, 0), maxRadiusScene),
        SCENE_EARTH_RADIUS * 1.3,
      );
    }
  }

  private onFrame(deltaSeconds: number): void {
    const simSeconds = this.simulation.tick(deltaSeconds);
    this.earthSpin.rotation.y = (simSeconds / EARTH_SIDEREAL_DAY_SECONDS) * Math.PI * 2;
    this.satelliteManager?.update(simSeconds);

    this.controls.update();
    this.renderer.render(this.scene, this.camera);
  }

  ngOnDestroy(): void {
    this.animationLoop?.stop();
    this.resizeHandler?.dispose();
    this.satelliteManager?.dispose();
    if (this.scene) disposeObject3D(this.scene);
    this.renderer?.dispose();
  }
}
