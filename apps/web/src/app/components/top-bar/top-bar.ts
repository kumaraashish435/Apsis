import { Component, input, output } from '@angular/core';

@Component({
  selector: 'app-top-bar',
  imports: [],
  templateUrl: './top-bar.html',
  styleUrl: './top-bar.scss',
})
export class TopBar {
  readonly title = input('APSIS');
  readonly subtitle = input('ORBITAL SIMULATOR');

  /** Drives the red badge dot on the status icon — e.g. active system alerts. */
  readonly alertCount = input(0);

  readonly statusClick = output<void>();
  readonly notificationsClick = output<void>();
  readonly historyClick = output<void>();
  readonly menuClick = output<void>();
}
