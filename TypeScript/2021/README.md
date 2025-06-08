# AoC TypeScript

## Concepts

- CommonJS

    CommonJS module may not support all module.exports as named exports:

    ❌

    ```js
    import {sync} from "fast-glob" 
    ```

    ✅

    ```js
    import fg from "fast-glob" 
    fg.sync(...)
    // extract
    const {sync} = fg
    sync(...) 
    ```

## npm packagename requirements

```shell
^(?:(?:@(?:[a-z0-9-*~][a-z0-9-*.*~]*)?/[a-z0-9-.*~])|[a-z0-9-~])[a-z0-9-.*~]*$
    [@[groupname]/]<packagename>
        start with @ or [a-z0-9-~]
        groupname:
            @ is followed by a group name in the form of @[groupname]/<packagename>
            groupname can be empty or start with [a-z0-9-*~] later on [a-z0-9-*.*~]
        packagename:
            if there is a @ then packagename can start with [a-z0-9-.*~], otherwise packagename must start with [a-z0-9-~]
            the later packagename can be [a-z0-9-.*~]
```

## Doc

[Mozilla JS](https://developer.mozilla.org/en-US/docs/Web/JavaScript)
[TypeScript](https://www.typescriptlang.org/docs/handbook/utility-types.html)
