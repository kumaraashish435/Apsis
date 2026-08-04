import { Component } from '@angular/core';
import { EarthView } from './earth-view/earth-view';

@Component({
  selector: 'app-orbit-viewer',
  imports: [EarthView],
  templateUrl: './viewer-3d.html',
  styleUrl: './viewer-3d.scss',
})
export class Viewer3d {}
