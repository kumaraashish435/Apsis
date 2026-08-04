import * as THREE from 'three';
import { TEXTURE_BASE_PATH } from '../utils/constants';

const loader = new THREE.TextureLoader();

/** Loads a color texture from public/textures/, with sRGB color space set —
 *  Three.js 0.150+ requires this explicitly or color maps render washed out. */
export function loadColorTexture(fileName: string, onLoad?: () => void): THREE.Texture {
  const texture = loader.load(
    `${TEXTURE_BASE_PATH}/${fileName}`,
    () => onLoad?.(),
    undefined,
    (err) => console.error(`Failed to load texture ${fileName}:`, err),
  );
  texture.colorSpace = THREE.SRGBColorSpace;
  return texture;
}

/** Loads a non-color (alpha/roughness/normal) texture — no color-space
 *  conversion. Fails silently: used for optional textures (e.g. clouds) that
 *  may not exist in public/textures/ yet, which shouldn't be a hard error. */
export function loadDataTexture(fileName: string, onLoad?: () => void): THREE.Texture {
  return loader.load(`${TEXTURE_BASE_PATH}/${fileName}`, () => onLoad?.(), undefined, () => {});
}
