import { Component } from '@angular/core';
import { Orbitcanvas } from '../../components/orbitcanvas/orbitcanvas';

@Component({
  selector: 'app-orbit',
  imports: [Orbitcanvas],
  templateUrl: './orbit.html',
  styleUrl: './orbit.scss',
})
export class Orbit {}
