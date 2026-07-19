import js from '@eslint/js'
import { node } from 'globals'
import { defineConfig } from 'eslint/config'
import ts from 'typescript-eslint'
import stylistic from '@stylistic/eslint-plugin'

/**
 * https://eslint.org/docs/latest/use/configure
 * /configuration-files#configuration-objects
 */
export default defineConfig([
    {
        ignores: ['dist/**/*', 'node_modules/**/*'],
    },
    {
        files: ['**/*.{js,mjs,cjs,ts,mts,cts}'],
        extends: [
            js.configs.recommended,
            stylistic.configs.recommended,
            ...ts.configs.recommended,
        ],
        languageOptions: {
            ecmaVersion: 'latest',
            globals: node,
        },
        plugins: {
            '@stylistic': stylistic,
        },
        rules: {
            // indention
            '@stylistic/indent': ['error', 4, {
                ObjectExpression: 'first',
            }],
            // whitespaces
            '@stylistic/no-multiple-empty-lines': ['error',
                { max: 1, maxEOF: 0 },
            ],
            '@stylistic/eol-last': ['error', 'always'],
            '@stylistic/no-trailing-spaces': ['error', {
                skipBlankLines: false,
                ignoreComments: false,
            }],
            '@stylistic/jsx-curly-spacing': ['error', {
                when: 'never',
            }],
            '@stylistic/template-curly-spacing': ['error', 'never'],
            '@stylistic/type-generic-spacing': ['error'],
            // 6.0.0: "@stylistic/type-generic-spacing":
            // ["error", {before: false, after: false}]
            '@stylistic/jsx-tag-spacing': ['error', {
                closingSlash: 'never',
                beforeSelfClosing: 'always',
                afterOpening: 'never',
                beforeClosing: 'never',
            }],
            '@stylistic/no-multi-spaces': ['error', {
                ignoreEOLComments: true,
            }],
            '@stylistic/function-call-spacing': [2, 'never'],
            '@stylistic/no-whitespace-before-property': [2],
            '@stylistic/arrow-spacing': [2, { before: true, after: true }],
            '@stylistic/spaced-comment': [2, 'always'],
            '@stylistic/space-before-function-paren': [2, {
                anonymous: 'always',
                named: 'never',
                asyncArrow: 'always',
                catch: 'always',
            }],
            '@stylistic/space-in-parens': [2, 'never'],
            '@stylistic/comma-spacing': [2, { after: true }],
            '@stylistic/space-infix-ops': [2, { int32Hint: false }],
            // newlines
            '@stylistic/jsx-curly-newline': [2, {
                multiline: 'consistent',
                singleline: 'consistent',
            }],
            '@stylistic/max-len': ['error', {
                code: 79,
            }],
            // jsx
            '@stylistic/jsx-self-closing-comp': [2, {
                component: true,
                html: true,
            }],
        },
    },
])
