import js from '@eslint/js'
import { node } from 'globals'
import { defineConfig } from 'eslint/config'
import { configs } from "typescript-eslint"

export default defineConfig([
	{
		files: ['**/*.{js,mjs,cjs,ts,mts,cts}'],
		ignores: ['dist'],
		extends: [js.configs.recommended, ...configs.recommended],
		// https://eslint.org/docs/latest/use/configure/configuration-files#configuration-objects
		languageOptions: {
			ecmaVersion: "latest",
			globals: node
		}
	}
])
