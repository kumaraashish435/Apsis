import {
  AfterViewInit,
  Component,
  ElementRef,
  OnDestroy,
  ViewChild
} from '@angular/core';

import { configureCesium } from '../../../cesium-config';


@Component({
  selector: 'app-orbitcanvas',
  imports: [],
  templateUrl: './orbitcanvas.html',
  styleUrl: './orbitcanvas.scss',
})
export class Orbitcanvas {
  private viewer?: import('cesium').Viewer;

  async ngAfterViewInit(): Promise<void> {
    configureCesium();

    const Cesium = await import('cesium');

    this.viewer = new Cesium.Viewer('cesiumContainer', {
      creditContainer: 'cesiumCredits',

      requestRenderMode: true,
      maximumRenderTimeChange: Infinity,

      animation: false,
      timeline: false,
      fullscreenButton: false,
      vrButton: false,
      geocoder: false,
      homeButton: false,
      sceneModePicker: false,
      navigationHelpButton: false,
      baseLayerPicker: false,
      infoBox: false,
      selectionIndicator: false,

      scene3DOnly: true
    });
  }

  ngOnDestroy(): void {
    this.viewer?.destroy();
  }

}