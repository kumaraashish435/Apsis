import { AfterViewInit, Component, ElementRef, NgZone, OnDestroy, ViewChild } from '@angular/core';
import * as THREE from 'three';
import { OrbitControls } from 'three-stdlib';

@Component({
  selector: 'app-earth-view',
  imports: [],
  templateUrl: './earth-view.html',
  styleUrl: './earth-view.scss',
})
export class EarthView implements AfterViewInit, OnDestroy {
  @ViewChild('canvasContainer')
  container!: ElementRef<HTMLDivElement>;

  private earthPivot!: THREE.Group;

  private scene!: THREE.Scene;
  private camera!: THREE.PerspectiveCamera;
  private renderer!: THREE.WebGLRenderer;
  private earth!: THREE.Mesh;
  private earthTexture!: THREE.Texture;

  private animationFrameId = 0;
  private resizeObserver!: ResizeObserver;
  private controls!: OrbitControls;

  private clock = new THREE.Clock();

  constructor(private readonly zone: NgZone) {}

  ngAfterViewInit() {
    const el = this.container.nativeElement;

    this.scene = new THREE.Scene();

    this.camera = new THREE.PerspectiveCamera(60, el.clientWidth / el.clientHeight, 0.1, 1000);
    this.camera.position.z = 3;

    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.renderer.setSize(el.clientWidth, el.clientHeight);
    el.appendChild(this.renderer.domElement);

    
    
    //camera controls

    this.controls = new OrbitControls(
        this.camera,
        this.renderer.domElement
    );

    this.controls.enableDamping = true;

    // Smooth movement
    this.controls.enableDamping = true;
    this.controls.dampingFactor = 0.05;

    // Limit zoom distance
    this.controls.minDistance = 2.5;
    this.controls.maxDistance = 10;

    // Disable panning
    this.controls.enablePan = false;

    // Always orbit around Earth's center
    this.controls.target.set(0, 0, 0);




    // Create the group that will hold the Earth and any other objects that should rotate with it.
    this.earthPivot = new THREE.Group();
    this.scene.add(this.earthPivot);

    this.earthPivot.rotation.z = THREE.MathUtils.degToRad(23.44);

    // Served from public/textures/ (Angular 22's default static-assets folder —
    // NOT src/assets/, which angular.json doesn't wire up by default).
    const textureLoader = new THREE.TextureLoader();
    this.earthTexture = textureLoader.load(
      '/textures/earth_day.jpg',
      () => console.log('Texture loaded'),
      undefined,
      (err) => console.error('Failed to load Earth texture:', err),
    );
    // Three.js 0.150+ removed the old `.encoding` API — a color texture needs
    // colorSpace set explicitly or it renders washed out.
    this.earthTexture.colorSpace = THREE.SRGBColorSpace;

    const geometry = new THREE.SphereGeometry(1, 64, 64);
    const material = new THREE.MeshPhongMaterial({ map: this.earthTexture });
    this.earth = new THREE.Mesh(geometry, material);
    this.earthPivot.add(this.earth);

    const ambientLight = new THREE.AmbientLight(0xffffff, 0.3);
    this.scene.add(ambientLight);

    const directionalLight = new THREE.DirectionalLight(0xffffff, 3);
    directionalLight.position.set(5, 3, 5);
    this.scene.add(directionalLight);



    // Run the render loop outside Angular's zone — requestAnimationFrame
    // at ~60fps would otherwise trigger a full change-detection pass on
    // every single frame for as long as this component is alive.
    this.zone.runOutsideAngular(() => this.animate());

    this.resizeObserver = new ResizeObserver(() => this.onResize());
    this.resizeObserver.observe(el);
  }

  ngOnDestroy() {
    cancelAnimationFrame(this.animationFrameId);
    this.resizeObserver?.disconnect();

    this.earth?.geometry.dispose();
    (this.earth?.material as THREE.Material)?.dispose();
    this.earthTexture?.dispose();
    this.renderer?.dispose();
  }

  private onResize() {
    const el = this.container.nativeElement;
    if (!el.clientWidth || !el.clientHeight) return;

    this.camera.aspect = el.clientWidth / el.clientHeight;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(el.clientWidth, el.clientHeight);
  }

  private animate = () => {
    this.animationFrameId = requestAnimationFrame(this.animate);
    this.controls.update();
    
    // One full rotation every 60 seconds
    // Set up the clock to track elapsed time for rotation
    const elapsed = this.clock.getElapsedTime();
    this.earth.rotation.y = elapsed * (Math.PI * 2) / 60;

    this.renderer.render(this.scene, this.camera);
  };
}
