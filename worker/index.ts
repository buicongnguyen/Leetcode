import handler from "vinext/server/app-router-entry";

interface Environment {
  ASSETS: Fetcher;
}

interface ExecutionContext {
  waitUntil(promise: Promise<unknown>): void;
  passThroughOnException(): void;
}

const worker = {
  fetch(
    request: Request,
    environment: Environment | undefined,
    context: ExecutionContext,
  ) {
    const url = new URL(request.url);
    if (url.pathname === "/book") {
      url.pathname = "/book/";
      return Response.redirect(url, 308);
    }
    if (url.pathname.startsWith("/book/")) {
      if (url.pathname.endsWith("/")) {
        url.pathname += "index.html";
      }
      const assetRequest = new Request(url, request);
      if (environment?.ASSETS) {
        return environment.ASSETS.fetch(assetRequest);
      }
      return handler.fetch(assetRequest, environment, context);
    }
    return handler.fetch(request, environment, context);
  },
};

export default worker;
