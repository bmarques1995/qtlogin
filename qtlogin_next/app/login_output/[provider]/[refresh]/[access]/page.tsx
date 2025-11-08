'use client';
import { useParams } from 'next/navigation';
import { useEffect } from 'react';
import { validOriginsReverse } from '@/shared/validOrigins';

export default function Home() {
    const params = useParams<{ refresh: string; access: string, provider: string }>();
    const sendResponseToQt = () => {
        const payload = {
            action: "register_tokens",
            item: {
                refresh: params.refresh,
                access: params.access
            }
        };

        // Encode JSON for URL (Base64 or percent)
        const json = encodeURIComponent(JSON.stringify(payload));
        const url = `${validOriginsReverse.get(params.provider)!}://open?data=${json}`;

        // Open the desktop app
        window.location.href = url;
    }

    console.log(validOriginsReverse.get(params.provider)!);
    console.log(params.refresh);
    console.log(params.access);

    useEffect(() => {

    },[]);

    return (
        <div className="flex flex-col min-h-screen items-center justify-center bg-zinc-50 font-sans dark:bg-black">
            <button type="button" style={{'cursor': 'pointer', 'border': '1px solid #fff', 'padding': '4px', 'margin': '4px'}} onClick={()=>sendResponseToQt()}>Enviar resposta</button>
        </div>
    );
}