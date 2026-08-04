import { AfterViewInit, Component, ElementRef, NgZone, OnDestroy, ViewChild } from '@angular/core';
import * as THREE from 'three';
import type { OrbitControls } from 'three-stdlib';

import { createScene } from './scene/scene';
import { createCamera } from './camera/camera';
import { createRenderer } from './renderer/renderer';
import { createOrbitControls } from './camera/orbit-controls';
import { ResizeHandler } from './renderer/resize';
import { AnimationLoop } from './renderer/animation-loop';
import { createEarth } from './earth/earth';
import { createLights } from './earth/lights';
import { Simulation } from './simulation/simulation';
import { EARTH_AXIAL_TILT_DEG, EARTH_SIDEREAL_DAY_SECONDS } from './utils/constants';
import { disposeObject3D } from './utils/helpers';

@Component({
  selector: 'app-earth-view',
  imports: [],
  templateUrl: './earth-view.html',
  styleUrl: './earth-view.scss',
})
export class EarthView implements AfterViewInit, OnDestroy {
  @ViewChild('canvasContainer')
  container!: ElementRef<HTMLDivElement>;

  private scene!: THREE.Scene;
  private camera!: THREE.PerspectiveCamera;
  private renderer!: THREE.WebGLRenderer;
  private controls!: OrbitControls;

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

  private readonly simulation = new Simulation(600);

  constructor(private readonly zone: NgZone) {}

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

    const { ambient, sun } = createLights();
    this.scene.add(ambient, sun);

    this.resizeHandler = new ResizeHandler(el, this.camera, this.renderer, () =>
      this.controls.update(),
    );

    // Run the render loop outside Angular's zone — requestAnimationFrame at
    // ~60fps would otherwise trigger a full change-detection pass on every
    // single frame for as long as this component is alive.
    this.zone.runOutsideAngular(() => {
      this.animationLoop = new AnimationLoop((deltaSeconds) => this.onFrame(deltaSeconds));
      this.animationLoop.start();
    });
  }

  private onFrame(deltaSeconds: number): void {
    const simSeconds = this.simulation.tick(deltaSeconds);
    this.earthSpin.rotation.y = (simSeconds / EARTH_SIDEREAL_DAY_SECONDS) * Math.PI * 2;

    this.controls.update();
    this.renderer.render(this.scene, this.camera);
  }

  ngOnDestroy(): void {
    this.animationLoop?.stop();
    this.resizeHandler?.dispose();
    if (this.scene) disposeObject3D(this.scene);
    this.renderer?.dispose();
  }
}
