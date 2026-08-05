import { Component } from '@angular/core';
import { EarthView } from './earth-view/earth-view';
import { TopBar } from '../../components/top-bar/top-bar';

@Component({
  selector: 'app-orbit-viewer',
  imports: [EarthView, TopBar],
  templateUrl: './viewer-3d.html',
  styleUrl: './viewer-3d.scss',
})
export class Viewer3d {}
