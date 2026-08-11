import { Component } from '@angular/core';
import { Sidebar } from '../../components/sidebar/sidebar';
import { Menu } from '../../components/menu/menu';
import { RouterOutlet } from '@angular/router';

@Component({
  selector: 'app-main',
  imports: [Sidebar, Menu, RouterOutlet],
  templateUrl: './main.html',
  styleUrl: './main.scss',
})
export class Main {}
