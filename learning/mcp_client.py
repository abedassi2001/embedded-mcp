
import asyncio
import sys
import json

from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client


async def main():

    # Start the MCP server as a separate Python process
    server = StdioServerParameters(
        command=sys.executable,
        args=["-m", "embedded_mcp.server"],
    )

    # Connect to the MCP server
    async with stdio_client(server) as (read, write):

        async with ClientSession(read, write) as session:

            # Initialize MCP connection
            await session.initialize()

            print("Connected to embedded-mcp!")

            # Discover available MCP tools
            tools = await session.list_tools()

            print("\nAvailable MCP tools:")
            for tool in tools.tools:
                print("-", tool.name)

            # Commands to test
            commands = [
                "STATUS",
                "PING",
                "GET_LED_STATE",
                "LED_ON",
                "GET_LED_STATE",
                "LED_OFF",
                "GET_LED_STATE",
                "HELLO",
            ]

            print("\n--- Testing Firmware Commands ---")

            for command in commands:

                # Execute the command through MCP
                result = await session.call_tool(
                    "send_command",
                    arguments={
                        "port": "/tmp/virtual_host",
                        "command": command,
                        "reply_seconds": 0.5,
                    },
                )

                if result.isError:
                    print(f"{command} -> MCP tool error")
                    continue

                # Extract the response returned by our MCP server
                for item in result.content:

                    if item.type == "text":

                        try:
                            data = json.loads(item.text)

                            if not data.get("ok", False):
                                print(
                                    f"{command} -> ERROR: "
                                    f"{data.get('error', 'Unknown error')}"
                                )
                            else:
                                reply = data.get("reply", "")
                                print(f"{command} -> {reply.strip()}")

                        except json.JSONDecodeError:
                            print(f"{command} -> {item.text}")


if __name__ == "__main__":
    asyncio.run(main())
